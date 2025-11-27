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

#define C2 36

const char scale_notes[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

const uint8_t scale_interval[] = {0, 2, 4, 5, 7, 9, 11};

static const ChordQuality MAJOR_SCALE[] = {
    // Major, minor, minor, Major, Major, minor, diminished
    {"",     {0,  4,  7},  3},
    {"m",    {2,  5,  9},  3}, 
    {"m",    {4,  7,  11}, 3}, 
    {"",     {5,  8,  12}, 3}, 
    {"",     {9,  13, 16}, 3}, 
    {"dim",  {11, 14, 17}, 3}  
};

static const ChordQuality MAJOR_SCALE_7[] = {
    {"maj7",     {0,  4,  7,  11},  4},
    {"m7",       {2,  5,  9,  12},  4}, 
    {"m7",       {4,  7,  11, 14},  4}, 
    {"maj7",     {5,  8,  12, 16},  4}, 
    {"maj7",     {9,  13, 16, 20},  4}, 
    {"dim7",     {11, 14, 17, 20},  4}  
};

static const ChordQuality MAJOR_SCALE_ADD9[] = {
    {"add9",        {0,  4,  7},  4},
    {"m(add9)",     {2,  5,  9},  4}, 
    {"m(add9)",     {4,  7,  11}, 4}, 
    {"add9",        {5,  8,  12}, 4}, 
    {"add9",        {9,  13, 16}, 4}, 
    {"dim(add9)",   {11, 15, 18}, 4}  
};

static const ChordQuality INV_SCALE_ADD9[] = {
    {"add9",        {0,  4,  7},  4},
    {"m(add9)",     {2,  5,  9},  4}, 
    {"m(add9)",     {4,  7,  11}, 4}, 
    {"add9",        {5,  8,  12}, 4}, 
    {"add9",        {9,  13, 16}, 4}, 
    {"dim(add9)",   {11, 15, 18}, 4}  
};

static const ChordQuality QUALITIES[] = {
    {"Major",     {0, 4, 7},         3}, // 000
    {"Minor",     {0, 3, 7},         3}, // 001
    {"Major 7",   {0, 4, 7, 11},     4}, // 010
    {"Minor 7",   {0, 3, 7, 10},     4}, // 011
    {"Add9",      {0, 4, 7, 14},     4}, // 100
    {"Minor Add9",{0, 3, 7, 14},     4}  // 101
    {"Major 9",   {0, 4, 7, 11, 14}, 5}, // 110
    {"Minor 9",   {0, 3, 7, 10, 14}, 5}, // 111
    {{"Diminished"},{}, }
};



#endif // MIDI_CHORDS_H