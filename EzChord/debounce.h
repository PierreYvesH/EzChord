#pragma once

#include <stdbool.h>
#include <stdint.h>

// EvenOdd debounce, based on the integrator algorithm by Kenneth A. Kuhn.
// Bit 0 holds the debounced state (1 = pressed), the upper bits a hysteresis
// counter. A zero-initialised Debounce is a released switch.
typedef struct {
    uint8_t state;
} Debounce;

typedef enum {
    DEBOUNCE_NONE,
    DEBOUNCE_PRESSED,
    DEBOUNCE_RELEASED,
} DebounceEdge;

// Feed one raw sample. An edge is reported once, after `ticks` net
// consistent samples that differ from the current state. ticks <= 127.
DebounceEdge debounce_update(Debounce *d, bool pressed, unsigned ticks);
