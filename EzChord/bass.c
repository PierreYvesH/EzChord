#include "EzChord/bass.h"
#include "EzChord/config.h"
#include "EzChord/debounce.h"
#include "EzChord/performance.h"
#include <pico/stdlib.h>

void bass_init(){
    gpio_init(BASS_PIN);
    gpio_set_dir(BASS_PIN, GPIO_IN);
    gpio_pull_up(BASS_PIN);
}

void bass_tick(){
    // V+ --- PULL UP ---- PIN -----./ .----- GND
    // If switch on, pin is shorted, signal = 0
    static Debounce debounce;
    switch (debounce_update(&debounce, !gpio_get(BASS_PIN), DEBOUNCE_TICKS)) {
        case DEBOUNCE_PRESSED:  performance_set_bass(performance(), true);  break;
        case DEBOUNCE_RELEASED: performance_set_bass(performance(), false); break;
        default: break;
    }
}
