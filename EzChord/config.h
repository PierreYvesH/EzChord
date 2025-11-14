#pragma once

// GPIO pins
// ---------

#define MATRIX_ROWS  2
#define MATRIX_COLS  3

#define MATRIX_ROW_PINS  { 0,1 }
#define MATRIX_COL_PINS  { 2,3,4 }

//Future for key change
//#define ENCODER_A_PIN  0
//#define ENCODER_B_PIN  1

// Midi usage
// ---------

#define MATRIX_BASE_LAYER  {\
    11, 4, 5,\
    12, 1, 6}

//#define ENCODER_CW_USAGE   cVOLUP
//#define ENCODER_CCW_USAGE  cVOLDN

// Timing
// ------

#define WATCHDOG_TIMEOUT_MS  100
#define PIN_SETTLE_TIME_US   10
#define TICK_INTERVAL_US     500

#define MATRIX_DEBOUNCE_TICKS   8
//#define ENCODER_KEYPRESS_TICKS  30
#define LED_BLINK_TICKS         500

//#define FN_KEY_TAPS    2
//#define BOOT_KEY_TAPS  3
