#pragma once

#include "EzChord/config.h"
#include <stdbool.h>
#include <stdint.h>

// QUALITY_* bits and CHORD_KEY live in quality_bits.h (included by config.h)

typedef struct {
    uint8_t notes[MAX_NOTE];  // MIDI notes, only the first `count` are valid
    uint8_t count;
    char name[16];
} Chord;

const char *chord_pitch_name(int pitch_class);

// Pure: no hardware. `key` indexes CHORD_LAYER, `held_quality` is OR-ed into
// the key's own quality. `octave` is -1..8 (2 = C2 base), `scale` is the
// transposition in semitones 0..11. Notes above 127 are dropped.
Chord chord_voice(uint8_t key, uint8_t held_quality, int octave, int scale);
