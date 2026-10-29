#pragma once

// Raw MIDI output over USB (channel 1).
void note_off(int note);
void note_on(int note, int velocity);

// Seam: where notes are sent. USB in firmware (midi_usb_out()), a
// recording fake in tests. Pure header: no USB stack needed to include it.
typedef struct {
    void *ctx;
    void (*note_on)(void *ctx, int note, int velocity);
    void (*note_off)(void *ctx, int note);
} MidiOut;

// The USB adapter: forwards to note_on()/note_off().
MidiOut midi_usb_out(void);
