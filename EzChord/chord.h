#pragma once

// Chord keys: a key matrix whose presses play a chord (see chord_voicing.h)
// through MIDI. Call chord_tick() every main-loop tick.
void chord_init();
void chord_tick();
// Name of the chord currently held, or NULL when none
const char *chord_display_name(void);
