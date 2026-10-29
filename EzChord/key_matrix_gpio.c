#include "EzChord/key_matrix_gpio.h"
#include "EzChord/config.h"
#include <pico/stdlib.h>

static void set_column(void *ctx, unsigned col, bool level)
{
    gpio_put(((GpioKeyMatrix *)ctx)->col_pins[col], level);
}

static bool read_row(void *ctx, unsigned row)
{
    return gpio_get(((GpioKeyMatrix *)ctx)->row_pins[row]);
}

static void settle(void *ctx)
{
    (void)ctx;
    sleep_us(PIN_SETTLE_TIME_US);
}

void gpio_key_matrix_init(GpioKeyMatrix *g,
                          const unsigned *row_pins, unsigned rows,
                          const unsigned *col_pins, unsigned cols,
                          KeyMatrixCallback on_key)
{
    //                           Pullup
    //                 Switch  .--[ R ]-- V+
    //          Diode  __|__   |
    // Column ---|<|---O   O---+--------- Row

    // Called by key_matrix_tick; ctx is the GpioKeyMatrix itself
    g->row_pins = row_pins;
    g->col_pins = col_pins;
    g->pins = (KeyMatrixPins){g, set_column, read_row, settle};

    // Rows: inputs with pull-ups. Columns: outputs, idle high.
    for (unsigned r = 0; r < rows; ++r) {
        gpio_init(row_pins[r]);
        gpio_set_dir(row_pins[r], GPIO_IN);
        gpio_pull_up(row_pins[r]);
    }
    for (unsigned c = 0; c < cols; ++c) {
        gpio_init(col_pins[c]);
        gpio_set_dir(col_pins[c], GPIO_OUT);
        gpio_put(col_pins[c], 1);
    }

    key_matrix_init(&g->matrix, &g->pins, rows, cols, on_key);
}

void gpio_key_matrix_tick(GpioKeyMatrix *g)
{
    key_matrix_tick(&g->matrix);
}
