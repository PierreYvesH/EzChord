#pragma once

#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>
#include <stdbool.h>

void set_scale(int scale);
int get_scale();
const char** get_scale_notes();
const uint8_t* get_scale_interval();
void set_octave(int oct);
int get_octave();
void set_bass(bool val);
int get_bass();
void get_chord_name(char *dest);
void note_off(int note);
void note_on(int note, int velocity);

uint8_t * midi_chord_notes(int root, bool minor, bool six, bool add7, bool add9, bool diminished);

void midi_chord_name(int root, bool minor, bool six, bool add7, bool add9, bool diminished);


#define C2 36

#endif