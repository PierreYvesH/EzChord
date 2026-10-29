#pragma once

#include <stdbool.h>

typedef enum {
    DISPLAY_FONT_BIG,    // chord name
    DISPLAY_FONT_SMALL,  // status line
} DisplayFont;

// Seam: where the layout is drawn. The OLED in firmware (oled.c),
// a recording fake in tests.
typedef struct {
    void *ctx;
    void (*draw_text)(void *ctx, int x, int y, DisplayFont font, const char *text);
    void (*draw_line)(void *ctx, int x0, int y0, int x1, int y1);
} DisplaySink;

// What the screen shows, gathered by the caller.
typedef struct {
    const char *chord_name;  // NULL when no chord is sounding
    bool bass;
    bool octave_mode;        // true: encoder changes octave, false: scale
    int octave;
    const char *scale_name;  // pitch name of the scale transposition, e.g. "C#"
} DisplayModel;

// Lays out one frame for a 128x64 screen and sends it to the sink.
// Chord name centered on top; below it "BASS ON/OFF" and "OCT +2  C#",
// with an underline marking the field the encoder currently changes.
void display_render(const DisplayModel *m, const DisplaySink *sink);
