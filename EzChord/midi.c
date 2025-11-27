#include "ezchord/midi.h"
#include "ezchord/config.h"
#include "tusb.h"
#include <stdint.h>

//static bool playing = false;
static uint8_t msg[3];
static uint8_t quality = 0;
static uint8_t octave = 2;
static uint8_t root = 0; 
static uint8_t notes[10] = {0};
static uint8_t scale_modifier = 0;

static ChordQuality chord;

static void set_scale(uint8_t scale){
    scale_modifier = scale;
}

static void set_octave(uint8_t oct){
    octave = oct;
} 


static void note_off(int note){
    msg[0] = 0x80;                    // Note Off - Channel 1
    msg[1] = note;
    msg[2] = 0;
    tud_midi_n_stream_write(0, 0, msg, 3); 
}

static void note_on(int note){
    // Then play
    msg[0] = 0x90;                    // Note On - Channel 1
    msg[1] = note;
    msg[2] = velocity;                // Velocity
    tud_midi_n_stream_write(0, 0, msg, 3); 
}

//returns notes based on root note
static uint8_t * midi_chord_notes(int root, bool minor, bool add7, bool add9, bool diminished){
    uint8_t[10] notes;
    return *notes
} 

static void report(uint k, bool signal)
{
    static const uint32_t layer[MATRIX_ROWS*MATRIX_COLS] = MATRIX_BASE_LAYER;
    const uint32_t active = layer[k];
    static const uint8_t velocity = 100;
    static uint8_t pressed_keys = 0;
    int note = 0;

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

        if (pressed_keys ==1){
            // Remove the codes
            for (int i = 0; i < 10; i++){
                if (notes[i]>0){
                    note_off(notes[i])
                    notes[i] = 0;
                }
            }   
        }
        pressed_keys -= 1;

    }

    // Note on
    else {
        // First remove any playing notes
        if (pressed_keys > 1){
            // Remove the codes
            for (int i = 0; i < 10; i++){
                if (notes[i]>0){
                    note_off(notes[i])
                    notes[i] = 0;
                }
            }
        }

        // Then play
        root = C2 + scaleInterval[k] + scale_modifier + 12 * (octave - 2);
        chord = QUALITIES[quality];
    
        for (unt i = 0; i < chord.num_notes; i++){
            note = root + chord.intervals[i];
            note_on(note);
            notes[i] = note;
        }
        pressed_keys += 1;
    }
}