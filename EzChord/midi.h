#pragma once

#include <stdint.h>

#ifndef MIDI_CHORDS_H
#define MIDI_CHORDS_H

static void change_scale(uint8_t scale);
static void report(uint key, bool signal);

// Chord quality defined by intervals from root
typedef struct {
    const char* name;
    uint8_t intervals[6];  // Semitones from root, 0-terminated
    uint8_t num_notes;
} ChordQuality;

const  uint8_t scaleInterval[] = {0, 2, 4, 5, 7, 9, 11};
static const ChordQuality QUALITIES[] = {
    {"Major",     {0, 4, 7},         3},
    {"Minor",     {0, 3, 7},         3},
    {"Major 7",   {0, 4, 7, 11},     4},
    {"Minor 7",   {0, 3, 7, 10},     4},
    {"Add9",      {0, 4, 7, 14},     4},
    {"Major 9",   {0, 4, 7, 11, 14}, 5},
    {"Minor 9",   {0, 3, 7, 10, 14}, 5},
    
};

#define C2 36

#endif // MIDI_CHORDS_H