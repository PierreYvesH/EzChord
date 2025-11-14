#include "ezchord/midi.h"
#include "ezchord/config.h"
#include "tusb.h"
#include <stdint.h>

//static bool playing = false;
static uint8_t msg[3];
static uint8_t quality = 0;
static uint8_t root = 0; 
static uint8_t notes[10] = {0};
static uint8_t scale_modifier = 0;

static ChordQuality chord;

static void change_scale(uint8_t scale){
    scale_modifier = scale;
}

static void report(uint k, bool signal)
{
    static const uint32_t layer[MATRIX_ROWS*MATRIX_COLS] = MATRIX_BASE_LAYER;
    const uint32_t active = layer[k];
    static const uint8_t velocity = 100;

    // Quality or not
    if (active > 9) { 
        // if pressed, signal = 0 if depressed, signal = 1.
        if (signal) { 
            quality -= (active - 10);
        }
        else {
            quality += (active - 10);
        }
    }
    // Note off
    else if (signal) {
        msg[0] = 0x80;                    // Note Off - Channel 1
        msg[2] = 0;                       // Velocity
 
        for (uint i = 0; i < 10; i++){
            if (notes[i]>0){
                msg[1] = notes[i];
                tud_midi_n_stream_write(0, 0, msg, 3); 
            }
        }
    }
    // Note on
    else {
        // Then play
        msg[0] = 0x90;                    // Note On - Channel 1
        msg[2] = velocity;                       // Velocity
        root = C2 + scaleInterval[k] + scale_modifier;
        chord = QUALITIES[quality];
    
        for (uint i = 0; i < chord.num_notes; i++){
            msg[1] = root + chord.intervals[i];
            notes[i] = msg[1];
            tud_midi_n_stream_write(0, 0, msg, 3); 
        }
    }
}