#include "EzChord/display_layout.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_W 128
#define SCREEN_H 64

#define BIG_CHAR_W 13
#define CHORD_Y 5

#define STATUS_X 2
#define STATUS_Y (SCREEN_H - 16)
// Gap between "BASS OFF" (the widest bass text) and the octave/scale text:
// 8 small characters of 7 px, minus 3 px kerning, plus 8 px padding.
#define OCT_SCALE_DX (8 * 7 - 3 + 8)

#define UNDERLINE_Y (SCREEN_H - 1)
#define UNDERLINE_OCT_LEN (7 * 5 + 4)    // under "OCT +n"
#define UNDERLINE_SCALE_DX (UNDERLINE_OCT_LEN + 8)
#define UNDERLINE_SCALE_LEN 14           // under the pitch name

void display_render(const DisplayModel *m, const DisplaySink *sink)
{
    if (m->chord_name) {
        // Centered; clamped so a long name never starts off-screen
        int x = (SCREEN_W - BIG_CHAR_W * (int)strlen(m->chord_name)) / 2;
        if (x < 0)
            x = 0;
        sink->draw_text(sink->ctx, x, CHORD_Y, DISPLAY_FONT_BIG, m->chord_name);
    }

    sink->draw_text(sink->ctx, STATUS_X, STATUS_Y, DISPLAY_FONT_SMALL,
                    m->bass ? "BASS ON" : "BASS OFF");

    // Sized for the longest case ("OCT -1  C#") with room to spare
    char oct_scale[24];
    snprintf(oct_scale, sizeof oct_scale, "OCT %s%d  %s",
             m->octave >= 0 ? "+" : "", m->octave, m->scale_name);
    const int oct_scale_x = STATUS_X + OCT_SCALE_DX;
    sink->draw_text(sink->ctx, oct_scale_x, STATUS_Y, DISPLAY_FONT_SMALL, oct_scale);

    int line_x = oct_scale_x;
    int line_len = UNDERLINE_OCT_LEN;
    if (!m->octave_mode) {
        line_x += UNDERLINE_SCALE_DX;
        line_len = UNDERLINE_SCALE_LEN;
    }
    sink->draw_line(sink->ctx, line_x, UNDERLINE_Y, line_x + line_len, UNDERLINE_Y);
}
