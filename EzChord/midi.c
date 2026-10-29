#include "EzChord/midi.h"
#include "tusb.h"
#include <stdint.h>

static uint8_t msg[3];

void note_off(int note){
    msg[0] = 0x80;                    // Note Off - Channel 1
    msg[1] = note;
    msg[2] = 0;
    tud_midi_n_stream_write(0, 0, msg, 3);
}

void note_on(int note, int velocity){
    msg[0] = 0x90;                    // Note On - Channel 1
    msg[1] = note;
    msg[2] = velocity;
    tud_midi_n_stream_write(0, 0, msg, 3);
}

// Adapters ignore ctx: the USB endpoint is a global
static void usb_note_on(void *ctx, int note, int velocity){
    (void)ctx;
    note_on(note, velocity);
}

static void usb_note_off(void *ctx, int note){
    (void)ctx;
    note_off(note);
}

MidiOut midi_usb_out(void){
    return (MidiOut){NULL, usb_note_on, usb_note_off};
}
