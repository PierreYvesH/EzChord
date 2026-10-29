#include "EzChord/performance.h"

void performance_init(Performance *p)
{
    p->octave = 2;
    p->scale = 0;
    p->bass = false;
    p->mode = PERFORMANCE_MODE_OCTAVE;
}

void performance_turn(Performance *p, int delta)
{
    if (p->mode == PERFORMANCE_MODE_OCTAVE) {
        p->octave += delta;
        if (p->octave < OCTAVE_MIN)
            p->octave = OCTAVE_MIN;
        else if (p->octave > OCTAVE_MAX)
            p->octave = OCTAVE_MAX;
    } else {
        // +12 keeps the modulo positive for negative steps
        p->scale = (p->scale + delta % 12 + 12) % 12;
    }
}

void performance_toggle_mode(Performance *p)
{
    p->mode = (p->mode == PERFORMANCE_MODE_OCTAVE) ? PERFORMANCE_MODE_SCALE
                                                    : PERFORMANCE_MODE_OCTAVE;
}

void performance_set_bass(Performance *p, bool on)
{
    p->bass = on;
}

Performance *performance(void)
{
    static Performance instance;
    static bool ready = false;
    if (!ready) {
        performance_init(&instance);
        ready = true;
    }
    return &instance;
}
