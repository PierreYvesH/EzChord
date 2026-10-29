#include "EzChord/config.h"
#include "EzChord/key_matrix_gpio.h"
#include "EzChord/quality.h"
#include "EzChord/quality_mask.h"

// Quality keys. Each key owns one or more QUALITY_* bits (QUALITY_LAYER);
// the mask is the OR of all keys held, read by chord.c on a chord press.
static const unsigned rowPins[QUALITY_ROWS] = QUALITY_ROW_PINS;
static const unsigned colPins[QUALITY_COLS] = QUALITY_COL_PINS;

static GpioKeyMatrix matrix;
static QualityMask held;
static const uint8_t layer[QUALITY_ROWS*QUALITY_COLS] = QUALITY_LAYER;

// Only records which keys are held; the mask is recomputed on read, so keys
// sharing a bit release correctly.
static void on_key(unsigned key, bool pressed)
{
    quality_mask_set(&held, key, pressed);
}

void quality_init()
{
    gpio_key_matrix_init(&matrix, rowPins, QUALITY_ROWS, colPins, QUALITY_COLS, on_key);
}

void quality_tick()
{
    gpio_key_matrix_tick(&matrix);
}

int get_quality()
{
    return quality_mask_get(&held, layer, QUALITY_ROWS*QUALITY_COLS);
}
