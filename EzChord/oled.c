#include "EzChord/config.h"
#include "EzChord/picoOled.h" 
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "EzChord/midi.h"
#include "EzChord/encoder.h"
#include "EzChord/small_font.h"
#include "EzChord/big_font.h"

#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>

void configOled(t_OledParams *oled);

static t_OledParams g_myOled;
static uint32_t oledTicks = OLED_TICKS;
static bool show_chord = false;

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


void set_show_chord(bool show){
    show_chord = show;
}
void oled_tick(){
    if (oledTicks++ < OLED_TICKS){
        return;
    }
    oledTicks = 0;

    oledClear(&g_myOled, BLACK);
    
    // Get Bass mode
    uint8_t bass = get_bass();
    
    // Get Oct/Scale mod Mode
    uint8_t mode = get_mode();
    
    // Get Oct and Scale
    int oct_n = get_octave();
    int scale_n = get_scale();
    const char** scale_notes = get_scale_notes();
    const char *scale = scale_notes[scale_n];
    
    char msg_oct_scale[10] = "OCT ";
    char msg_bass[10] = "";

    //Format
    char oct[3];
    if (oct_n >= 0) {
        sprintf(oct, "+%i", oct_n);

    } else {
        sprintf(oct, "%i", oct_n);
    }

    if (bass){
        strcat(msg_bass, "BASS ON");
    }else{
        strcat(msg_bass, "BASS OFF");
    }
    
    strcat(msg_oct_scale, oct);
    strcat(msg_oct_scale, "  ");
    strcat(msg_oct_scale, scale);

    
    // Display
    if (show_chord){
        char msg_chord[20];
        get_chord_name(msg_chord);
        uint8_t len = strlen(msg_chord);
        uint8_t chord_x = (W_128-13*len)/2;
        uint8_t chord_y = 5;
        oledDraw_string_font(&g_myOled, chord_x, chord_y,
                (const char *)msg_chord, &big_font_charmap[0], 
                &big_font[0]); 
    }

    uint8_t bot_x = 2;
    uint8_t bot_y = H_64 - 16;
    
    int shift = strlen("BASS OFF")*7-3+8;

    oledDraw_string_font(&g_myOled, bot_x, bot_y,
                (const char *)msg_bass, &small_font_charmap[0], 
                &small_font[0]); 
    oledDraw_string_font(&g_myOled, bot_x+shift, bot_y,
                (const char *)msg_oct_scale, &small_font_charmap[0], 
                &small_font[0]);
    
    uint8_t l_x = bot_x + shift;
    uint8_t l_len = 7 * 5 + 4;
    if (!mode){
        l_x += l_len + 8; 
        l_len = 14;
    }
    oledDraw_line(&g_myOled, l_x, H_64-1, l_x + l_len, H_64-1, WHITE);
    
    oledDisplay(&g_myOled);
    //printf("Time elapsed: %i ms \n", (int) (get_absolute_time() - time)/1000);
}
    