#pragma once

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

// chord: 0x degree, diminished (16), add9(8), add7(4), six(2), minor(1) 
// degrees: I, ii, iii, IV, V, vi, vii* - 1
#define CHORD_LAYER  {\
    0b00100001, 0b01100000, 0b10100001, 0x0, \
    0b00000000, 0b01000001, 0b10000000, 0b11010000}

// diminished(16), add9(8), add7(4), six (2), minor (1)
#define QUALITY_LAYER {\
    0b00010, 0b00100, 0b01000}

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
