#include "EzChord/chord.h"
#include "EzChord/chord_player.h"
#include "EzChord/config.h"
#include "EzChord/key_matrix_gpio.h"
#include "EzChord/midi.h"
#include "EzChord/performance.h"
#include "EzChord/quality.h"

// Chord keys: wiring only. The matrix reports debounced edges; the
// ChordPlayer decides what they mean. This file gathers the live inputs
// (quality keys, Performance settings) and connects the USB MIDI adapter.
static const unsigned rowPins[CHORD_ROWS] = CHORD_ROW_PINS;
static const unsigned colPins[CHORD_COLS] = CHORD_COL_PINS;

static GpioKeyMatrix matrix;
static ChordPlayer player;

static void on_key(unsigned key, bool pressed)
{
    // Sampled at the moment of the key edge
    const Performance *perf = performance();
    ChordContext ctx = {get_quality(), perf->octave, perf->scale, perf->bass};
    chord_player_key(&player, key, pressed, ctx);
}

void chord_init()
{
    chord_player_init(&player, midi_usb_out(), VEL);
    gpio_key_matrix_init(&matrix, rowPins, CHORD_ROWS, colPins, CHORD_COLS, on_key);
}

void chord_tick()
{
    gpio_key_matrix_tick(&matrix);
}

const char *chord_display_name(void)
{
    return chord_player_name(&player);
}
