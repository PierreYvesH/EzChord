#pragma once

#include "EzChord/quality_bits.h"

// GPIO pins
// ---------

#define CHORD_ROWS  2
#define CHORD_COLS  4

#define CHORD_ROW_PINS  { 0, 1 }
#define CHORD_COL_PINS  { 2, 3, 4, 5}

#define QUALITY_ROWS  1
#define QUALITY_COLS  4

#define QUALITY_ROW_PINS {6}
#define QUALITY_COL_PINS {7, 8, 9}

#define BASS_PIN 16

#define LED_PIN 25

// Future for key or basschange
#define ENCODER_A_PIN    13
#define ENCODER_B_PIN    12
#define ENCODER_SW_PIN   11

// OLED display
#define OLED_SDA_PIN 20
#define OLED_SCL_PIN 21
#define OLED_TICKS   200

#define VEL 100
// Midi usage
// ---------

// Chord keys: CHORD_KEY(degree, own quality bits), see quality_bits.h.
// degrees (0-based): I, ii, iii, IV, V, vi, vii*
#define CHORD_LAYER  {\
    CHORD_KEY(1, QUALITY_MINOR), CHORD_KEY(3, 0), \
    CHORD_KEY(5, QUALITY_MINOR), CHORD_KEY(0, 0), \
    CHORD_KEY(0, 0), CHORD_KEY(2, QUALITY_MINOR), \
    CHORD_KEY(4, 0), CHORD_KEY(6, QUALITY_DIM)}

// Quality keys: the QUALITY_* bits each key adds while held
#define QUALITY_LAYER {\
    QUALITY_SIX, QUALITY_ADD7, QUALITY_ADD9}

#define MAX_NOTE 6
// Timing
// ------

#define WATCHDOG_TIMEOUT_MS  100
#define PIN_SETTLE_TIME_US   10
#define TICK_INTERVAL_US     500

#define DEBOUNCE_TICKS   8
#define ENCODER_KEYPRESS_TICKS  30
#define LED_BLINK_TICKS         500           

//#define FN_KEY_TAPS    2
//#define BOOT_KEY_TAPS  3
