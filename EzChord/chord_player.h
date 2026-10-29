#pragma once

#include "EzChord/chord_voicing.h"
#include "EzChord/midi.h"
#include <stdbool.h>
#include <stdint.h>

// Everything that shapes a chord at the moment a key is pressed.
// Passed in explicitly so the player has no hidden inputs.
typedef struct {
    uint8_t quality;  // held QUALITY_* bits
    int octave;
    int scale;
    bool bass;        // true if the bass key is held
} ChordContext;

typedef struct {
    MidiOut out;
    int velocity;
    uint8_t playing[MAX_NOTE];  // notes currently sounding, to switch off exactly
    uint8_t playing_count;
    int last_pressed;           // key that owns the sounding chord, -1 if none
    bool showing;
    char name[16];
} ChordPlayer;

void chord_player_init(ChordPlayer *p, MidiOut out, int velocity);

// Feed one debounced key edge. Last press wins: a press replaces the
// sounding chord; a release only silences it if it is the key last pressed.
// `ctx` is read on press only and ignored on release.
void chord_player_key(ChordPlayer *p, unsigned key, bool pressed, ChordContext ctx);

// Name of the chord sounding now, or NULL when silent.
const char *chord_player_name(const ChordPlayer *p);
