#include "EzChord/config.h"
#include "EzChord/picoOled.h" 
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include <string.h>
#include "EzChord/performance.h"
#include "EzChord/chord_voicing.h"
#include "EzChord/chord.h"
#include "EzChord/display_layout.h"
#include "EzChord/small_font.h"
#include "EzChord/big_font.h"

void configOled(t_OledParams *oled);

static t_OledParams g_myOled;

// Last drawn model, for change detection. chord_name/scale_name pointers are
// not stable (player buffer / lookup), so the chord name is copied.
static DisplayModel last_model;
static char last_chord_name[16];
static bool drawn = false;

// True if `a` would render the same screen as `b` (whose chord name text
// is in `b_chord`, NULL pointer in b meaning no chord).
static bool model_equal(const DisplayModel *a, const DisplayModel *b, const char *b_chord){
    if ((a->chord_name == NULL) != (b->chord_name == NULL))
        return false;
    if (a->chord_name && strcmp(a->chord_name, b_chord) != 0)
        return false;
    return a->bass == b->bass
        && a->octave_mode == b->octave_mode
        && a->octave == b->octave
        && strcmp(a->scale_name, b->scale_name) == 0;
}

// Initialize the oled structure with the information
//  required to define the display and then configure it
void configOled(t_OledParams *oled){

    oled->i2c         = i2c0;
    oled->SDA_PIN     = OLED_SDA_PIN;
    oled->SCL_PIN     = OLED_SCL_PIN;

    oled->ctlrType    = CTRL_SH1106;
    oled->i2c_address = 0x3C;
    oled->height      = H_64;
    oled->width       = W_132;

// this will configure the OLED module and then
//  clear the screen.
    oledI2cConfig(oled);

}

void oled_init(){
    configOled(&g_myOled);
}

// OLED adapter for DisplaySink: maps the layout's fonts and lines onto
// the picoOled driver. ctx is the t_OledParams.
static void oled_draw_text(void *ctx, int x, int y, DisplayFont font, const char *text){
    if (font == DISPLAY_FONT_BIG) {
        oledDraw_string_font((t_OledParams *)ctx, x, y, (const uint8_t *)text,
                &big_font_charmap[0], &big_font[0]);
    } else {
        oledDraw_string_font((t_OledParams *)ctx, x, y, (const uint8_t *)text,
                &small_font_charmap[0], &small_font[0]);
    }
}

static void oled_draw_line(void *ctx, int x0, int y0, int x1, int y1){
    oledDraw_line((t_OledParams *)ctx, x0, y0, x1, y1, WHITE);
}

// Redraws the screen only when the displayed model changes, so the slow
// blocking I2C transfer doesn't stall the main loop (and miss encoder edges)
// while nothing is happening. All layout lives in display_layout.c; this
// gathers the model, compares it with the last drawn one, and drives the display.
void oled_tick(){
    // Snapshot of the live settings
    const Performance *perf = performance();
    const DisplayModel model = {
        .chord_name = chord_display_name(),
        .bass = perf->bass,
        .octave_mode = perf->mode == PERFORMANCE_MODE_OCTAVE,
        .octave = perf->octave,
        .scale_name = chord_pitch_name(perf->scale),
    };

    if (drawn && model_equal(&model, &last_model, last_chord_name))
        return;

    const DisplaySink sink = {&g_myOled, oled_draw_text, oled_draw_line};

    oledClear(&g_myOled, BLACK);
    display_render(&model, &sink);
    oledDisplay(&g_myOled);

    // chord_name points into the player's buffer, so keep a copy to compare
    last_model = model;
    if (model.chord_name) {
        strncpy(last_chord_name, model.chord_name, sizeof last_chord_name - 1);
        last_chord_name[sizeof last_chord_name - 1] = '\0';
    }
    drawn = true;
}
