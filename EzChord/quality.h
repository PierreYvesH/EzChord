#pragma once

// Quality keys: a key matrix of modifier keys (minor, six, add7, add9, dim).
// Call quality_tick() every main-loop tick.
void quality_init();
void quality_tick();
// Bit mask of the quality keys currently held (QUALITY_* bits, chord_voicing.h)
int get_quality();