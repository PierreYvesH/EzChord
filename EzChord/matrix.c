#include "ezchord/matrix.h"
#include "ezchord/config.h"
#include "ezchord/midi.h"
#include "ezchord/led.h"
#include "tusb.h"
#include <pico/bootrom.h> 
#include <pico/stdlib.h>
#include <stdint.h>

static_assert(MATRIX_DEBOUNCE_TICKS <= UINT8_MAX/2);

static const uint rowPins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const uint colPins[MATRIX_COLS] = MATRIX_COL_PINS;

static void debounce(uint key, bool signal);


void matrix_init()
{
    //                           Pullup
    //                 Switch  .--[ R ]-- V+
    //          Diode  __|__   |
    // Column ---|<|---O   O---+--------- Row

    for (uint r = 0; r < MATRIX_ROWS; ++r) {
        const uint pin = rowPins[r];
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
    }

    for (uint c = 0; c < MATRIX_COLS; ++c) {
        const uint pin = colPins[c];
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, 1);
    }
}


void matrix_tick()
{
    // The rows are pulled up and therefore read 1 by default.
    // All Columns are set to 1. During the scan, one column at a time
    // is set to 0. If a switch is pressed, the connected row
    // is grounded and reads 0 as well.

    for (uint c = 0; c < MATRIX_COLS; ++c) {
        gpio_put(colPins[c], 0);
        sleep_us(PIN_SETTLE_TIME_US);

        for (uint r = 0; r < MATRIX_ROWS; ++r) {
            const uint key = r*MATRIX_COLS + c;
            const bool signal = gpio_get(rowPins[r]);
            debounce(key, signal);
        }

        gpio_put(colPins[c], 1);
    }
}



static void debounce(uint key, bool signal)
{
    // EvenOdd debounce algorithm
    // Based on the integrator algorithm by Kenneth A. Kuhn
    // https://www.kennethkuhn.com/electronics/debounce.c
    //
    // The progress is maintained in one byte per switch.
    // The lowest bit represents the inverse of the current
    // switch state. It is inverted so that the starting
    // value is zero and can be auto-initalized in an array.
    // The remaining bits are used as a hysterisis counter.
    //
    // .-------------.-.
    // |7 6 5 4 3 2 1|0|
    // '-------------'-'
    // Hysterisis    Inverted
    // counter       switch state

    static uint8_t states[MATRIX_ROWS*MATRIX_COLS] = {0};
    uint8_t state = states[key];

    // The hysterisis counter reflects how often the signal
    // has matched the state bit in recent history. We add
    // or subtract a value of 2 to jump over the state bit.
    //            .-.
    //          .-' '-. .- Counter
    //    .-. .-'     '-'
    // ---' '-'
    // ! ! = ! = = = ! ! = Signal

    if (signal == state % 2)
        state += 2;
    else if (state >= 2)
        state -= 2;

    // When the signal has fully saturated the counter,
    // invert the state bit and reset the counter.

    if (state >= MATRIX_DEBOUNCE_TICKS * 2) {
        report(key, signal);
        state = !signal;
    }

    states[key] = state;
}



