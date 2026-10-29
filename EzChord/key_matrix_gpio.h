#pragma once

#include "EzChord/key_matrix.h"

// GPIO adapter for KeyMatrix (Pico SDK).
typedef struct {
    KeyMatrix matrix;
    KeyMatrixPins pins;
    const unsigned *row_pins;
    const unsigned *col_pins;
} GpioKeyMatrix;

void gpio_key_matrix_init(GpioKeyMatrix *g,
                          const unsigned *row_pins, unsigned rows,
                          const unsigned *col_pins, unsigned cols,
                          KeyMatrixCallback on_key);
void gpio_key_matrix_tick(GpioKeyMatrix *g);
