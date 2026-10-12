#include "EzChord/bass.h"
#include "EzChord/config.h"
#include "EzChord/midi.h"
#include <pico/stdlib.h>

static_assert(DEBOUNCE_TICKS <= UINT8_MAX/2);

void bass_init(){
    gpio_init(BASS_PIN);
    gpio_set_dir(BASS_PIN, GPIO_IN);
    gpio_pull_up(BASS_PIN);
}

void bass_tick(){
    // V+ --- PULL UP ---- PIN -----./ .----- GND 
    //If switch on, pin is shorted, signal = 0
    const bool signal = gpio_get(BASS_PIN);
    static uint8_t state = 0;
    if (signal == state % 2){
        state+=2;
    } else if(state >=2){
        state -= 2;
    }

    if (state >= DEBOUNCE_TICKS * 2) {
        set_bass((bool) !signal);
        state = !signal;
    }   
}