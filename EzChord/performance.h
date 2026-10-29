#pragma once

#include <stdbool.h>

#define OCTAVE_MIN (-1)
#define OCTAVE_MAX 8

// What turning the encoder changes
typedef enum {
    PERFORMANCE_MODE_OCTAVE,
    PERFORMANCE_MODE_SCALE,
} PerformanceMode;

// The player's live settings. Plain data: read the fields directly,
// change them only through the functions below so the rules hold.
typedef struct {
    int octave;            // OCTAVE_MIN..OCTAVE_MAX, 2 = C2 base
    int scale;             // transposition in semitones, 0..11
    bool bass;             // bass switch on
    PerformanceMode mode;  // what performance_turn() changes
} Performance;

// Defaults: octave 2, scale 0, bass off, octave mode.
void performance_init(Performance *p);

// One encoder step (+1 or -1) applied to the current mode.
// Octave clamps at its limits; scale wraps around 11 <-> 0.
void performance_turn(Performance *p, int delta);

void performance_toggle_mode(Performance *p);
void performance_set_bass(Performance *p, bool on);

// The firmware's single live instance, shared by encoder, bass switch,
// chord keys and display. Initialised on first use.
Performance *performance(void);
