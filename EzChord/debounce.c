#include "EzChord/debounce.h"

DebounceEdge debounce_update(Debounce *d, bool pressed, unsigned ticks)
{
    uint8_t state = d->state;

    // The counter rises while the sample disagrees with the debounced
    // state and falls while it agrees. +/-2 jumps over the state bit.
    if (pressed != (state & 1))
        state += 2;
    else if (state >= 2)
        state -= 2;

    if (state >= ticks * 2) {
        d->state = pressed;
        return pressed ? DEBOUNCE_PRESSED : DEBOUNCE_RELEASED;
    }

    d->state = state;
    return DEBOUNCE_NONE;
}
