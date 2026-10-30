#include "EzChord/chord_voicing.h"
#include <string.h>

#define BASE_NOTE 36  // C2

static const uint8_t layer[CHORD_ROWS * CHORD_COLS] = CHORD_LAYER;
// Semitones above the scale root for degrees I..vii (major scale)
static const uint8_t degree_interval[] = {0, 2, 4, 5, 7, 9, 11};
static const char *const pitch_names[12] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

const char *chord_pitch_name(int pitch_class)
{
    return pitch_names[pitch_class % 12];
}

// Name = pitch + quality + extension, e.g. "C", "Cm", "Cdim", "Cmaj7",
// "Cm9", "Cadd9", "Cmaj7/6". Minor wins over dim when both bits are set.
static void build_name(char *dest, int pitch_class, uint8_t q)
{
    const bool minor = q & QUALITY_MINOR;
    const bool dim = q & QUALITY_DIM;
    const bool six = q & QUALITY_SIX;
    const bool add7 = q & QUALITY_ADD7;
    const bool add9 = q & QUALITY_ADD9;

    strcpy(dest, chord_pitch_name(pitch_class));
    if (minor)
        strcat(dest, "m");
    else if (dim)
        strcat(dest, "dim");

    if (add7 && add9)
        strcat(dest, (minor || dim) ? "9" : "maj9");
    else if (add7)
        strcat(dest, (minor || dim) ? "7" : "maj7");
    else if (add9)
        strcat(dest, "add9");

    if (six)
        strcat(dest, (add7||add9) ? "/6" : "6");
}

Chord chord_voice(uint8_t key, uint8_t held_quality, int octave, int scale, bool bass)
{
    Chord chord = {0};
    // Layer byte: top 3 bits = degree, low 5 bits = the key's own quality
    const uint8_t active = layer[key % (CHORD_ROWS * CHORD_COLS)];
    const uint8_t degree = CHORD_KEY_DEGREE(active);
    const uint8_t q = CHORD_KEY_QUALITY(active) | held_quality;

    const bool minor = q & QUALITY_MINOR;
    const bool dim = q & QUALITY_DIM;
    // Offsets from the root of the chord, in semitones. Minor and dim flatten
    // the third; dim also flattens the fifth and lowers the 7th and 9th.
    const int third = (minor || dim) ? 3 : 4;
    const int fifth = dim ? 6 : 7;
    const int seventh = dim ? 9 : (minor ? 10 : 11);
    const int ninth = dim ? 13 : 14;

    // `offset` (scale + degree) is octave-independent, so it also gives the
    // pitch class used for the name.
    const int offset = scale + degree_interval[degree % 7];
    const int root = BASE_NOTE + (octave - 2) * 12 + offset;
    const int bass_note = bass ? root - 12 : root;

    // Candidates are built first, then range-checked so out-of-range notes
    // are dropped and `count` stays accurate.
    int candidates[MAX_NOTE];
    int n = 0;
    candidates[n++] = root;
    candidates[n++] = root + third;
    candidates[n++] = root + fifth;
    if (q & QUALITY_SIX)
        candidates[n++] = root + 9;
    if (q & QUALITY_ADD7)
        candidates[n++] = root + seventh;
    if (q & QUALITY_ADD9)
        candidates[n++] = root + ninth;
    if (bass)
        candidates[n++] = bass_note;

    for (int i = 0; i < n; i++) {
        if (candidates[i] >= 0 && candidates[i] <= 127)
            chord.notes[chord.count++] = (uint8_t)candidates[i];
    }

    build_name(chord.name, offset % 12, q);
    return chord;
}
