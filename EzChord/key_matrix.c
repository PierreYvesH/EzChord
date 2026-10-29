#include "EzChord/key_matrix.h"
#include "EzChord/config.h"
#include <string.h>

_Static_assert(DEBOUNCE_TICKS <= 127, "debounce counter is one byte");

void key_matrix_init(KeyMatrix *m, const KeyMatrixPins *pins,
                     unsigned rows, unsigned cols, KeyMatrixCallback on_key)
{
    memset(m, 0, sizeof *m);
    m->pins = pins;
    m->rows = rows;
    m->cols = cols;
    m->on_key = on_key;
}

void key_matrix_tick(KeyMatrix *m)
{
    // Rows are pulled up, so they read 1 by default. One column at a time is
    // driven low; a pressed key connects its row to that column, so the row
    // reads 0 too.
    for (unsigned c = 0; c < m->cols; ++c) {
        m->pins->set_column(m->pins->ctx, c, false);
        // Let the line settle before reading the rows
        m->pins->settle(m->pins->ctx);

        for (unsigned r = 0; r < m->rows; ++r) {
            const unsigned key = r * m->cols + c;
            const bool pressed = !m->pins->read_row(m->pins->ctx, r);
            const DebounceEdge edge =
                debounce_update(&m->keys[key], pressed, DEBOUNCE_TICKS);
            // Only debounced edges reach the caller, never raw samples
            if (edge != DEBOUNCE_NONE)
                m->on_key(key, edge == DEBOUNCE_PRESSED);
        }

        m->pins->set_column(m->pins->ctx, c, true);
    }
}
