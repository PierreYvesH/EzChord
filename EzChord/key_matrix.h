#pragma once

#include "EzChord/debounce.h"
#include <stdbool.h>

#define KEY_MATRIX_MAX_KEYS 16

// Seam: how the matrix touches pins. Rows are pulled up (read 1 when open);
// the matrix drives one column low at a time, so a pressed key reads 0.
typedef struct {
    void *ctx;
    void (*set_column)(void *ctx, unsigned col, bool level);
    bool (*read_row)(void *ctx, unsigned row);
    void (*settle)(void *ctx);
} KeyMatrixPins;

typedef void (*KeyMatrixCallback)(unsigned key, bool pressed);

typedef struct {
    const KeyMatrixPins *pins;
    unsigned rows, cols;
    KeyMatrixCallback on_key;
    Debounce keys[KEY_MATRIX_MAX_KEYS];
} KeyMatrix;

// key index = row * cols + col. Callback fires once per debounced edge,
// in scan order. All keys start released.
void key_matrix_init(KeyMatrix *m, const KeyMatrixPins *pins,
                     unsigned rows, unsigned cols, KeyMatrixCallback on_key);
void key_matrix_tick(KeyMatrix *m);
