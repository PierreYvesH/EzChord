#pragma once

#include <stdint.h>

#ifndef MIDI_CHORDS_H
#define MIDI_CHORDS_H

static void set_scale(uint8_t scale);
static void set_octave(uint8_t oct);
static void note_off(int note);
static void note_on(int note)

static void report(uint key, bool signal);

// Chord quality defined by intervals from root
typedef struct {
    const char* name;
    uint8_t intervals[6];  // Semitones from root, 0-terminated
    uint8_t num_notes;
} ChordQuality;

const  uint8_t scaleInterval[] = {0, 2, 4, 5, 7, 9, 11};
static const ChordQuality QUALITIES[] = {
    {"Major",     {0, 4, 7},         3}, // 000
    {"Minor",     {0, 3, 7},         3}, // 001
    {"Major 7",   {0, 4, 7, 11},     4}, // 010
    {"Minor 7",   {0, 3, 7, 10},     4}, // 011
    {"Add9",      {0, 4, 7, 14},     4}, // 100
    {"Minor Add9",{0, 3, 7, 14},     4}  // 101
    {"Major 9",   {0, 4, 7, 11, 14}, 5}, // 110
    {"Minor 9",   {0, 3, 7, 10, 14}, 5}, // 111
};

#define C2 36

#endif // MIDI_CHORDS_H