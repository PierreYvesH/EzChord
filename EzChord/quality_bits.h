#pragma once

#include <stdint.h>

// Quality bits: the single source of the bit layout. Used by CHORD_LAYER and
// QUALITY_LAYER (config.h) and decoded by chord_voicing.c.
#define QUALITY_MINOR  (1 << 0)
#define QUALITY_SIX    (1 << 1)
#define QUALITY_ADD7   (1 << 2)
#define QUALITY_ADD9   (1 << 3)
#define QUALITY_DIM    (1 << 4)

// All quality bits live in the low 5 bits of a CHORD_LAYER entry.
#define QUALITY_MASK   0x1F

// A CHORD_LAYER entry: degree (0..6) in the top 3 bits, the key's own
// quality bits in the low 5.
#define CHORD_KEY(degree, quality)  ((uint8_t)(((degree) << 5) | ((quality) & QUALITY_MASK)))
#define CHORD_KEY_DEGREE(entry)     ((entry) >> 5)
#define CHORD_KEY_QUALITY(entry)    ((entry) & QUALITY_MASK)
