#pragma once

// GPIO pins
// ---------

#define CHORD_ROWS  2
#define CHORD_COLS  4

#define CHORD_ROW_PINS  { 0, 1 }
#define CHORD_COL_PINS  { 2, 3, 4, 5}

#define QUALITIES_ROWS  1
#define QUALITIES_COLS  4

#define QUALITIES_ROW_PINS {6}
#define QUALITIES_COL_PINS {7, 8, 9, 10}

#define BASS_PIN 16

// Future for key or basschange
//#define ENCODER_A_PIN    11
//#define ENCODER_B_PIN    12
//#define ENCODER_PUSH_PIN 13

// OLED display
//#define OLED_SDA_PIN 14
//#define OLED_SCL_PIN 15

// Midi usage
// ---------

// chord: 0x degree, diminished, add9, add7, minor
#define CHORD_LAYER  {\
    0x20001, 0x40000, 0x60001, 0x0, \
    0x10000, 0x30001, 0x50000, 0x71001}

#define QUALITIES_LAYER {\
    0x0001, 0x0010, 0x0100, 0x1000}

//#define ENCODER_CW_USAGE   cVOLUP
//#define ENCODER_CCW_USAGE  cVOLDN

// Timing
// ------

#define WATCHDOG_TIMEOUT_MS  100
#define PIN_SETTLE_TIME_US   10
#define TICK_INTERVAL_US     500

#define DEBOUNCE_TICKS   8
//#define ENCODER_KEYPRESS_TICKS  30
#define LED_BLINK_TICKS         500

//#define FN_KEY_TAPS    2
//#define BOOT_KEY_TAPS  3
