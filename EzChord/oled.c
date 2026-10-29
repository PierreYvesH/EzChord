#include "EzChord/config.h"
#include "EzChord/picoOled.h" 
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "EzChord/performance.h"
#include "EzChord/chord_voicing.h"
#include "EzChord/chord.h"
#include "EzChord/display_layout.h"
#include "EzChord/small_font.h"
#include "EzChord/big_font.h"

void configOled(t_OledParams *oled);

static t_OledParams g_myOled;
static uint32_t oledTicks = OLED_TICKS;

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

// Redraws the screen every OLED_TICKS ticks. All layout lives in
// display_layout.c; this only gathers the model and drives the display.
void oled_tick(){
    if (oledTicks++ < OLED_TICKS){
        return;
    }
    oledTicks = 0;

    // One consistent snapshot of the live settings
    const Performance *perf = performance();
    const DisplayModel model = {
        .chord_name = chord_display_name(),
        .bass = perf->bass,
        .octave_mode = perf->mode == PERFORMANCE_MODE_OCTAVE,
        .octave = perf->octave,
        .scale_name = chord_pitch_name(perf->scale),
    };
    const DisplaySink sink = {&g_myOled, oled_draw_text, oled_draw_line};

    oledClear(&g_myOled, BLACK);
    display_render(&model, &sink);
    oledDisplay(&g_myOled);
}
