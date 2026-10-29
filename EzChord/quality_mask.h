#pragma once

#include <stdbool.h>
#include <stdint.h>

// Pure: tracks which quality keys are held and the OR of their bits.
// Recomputed from the held keys, so keys sharing a bit stay correct on release.
#define QUALITY_MASK_MAX_KEYS 8

typedef struct {
    uint8_t held;  // bit n set = key n is held
} QualityMask;

// Record key `key` (< QUALITY_MASK_MAX_KEYS) as held or released.
void quality_mask_set(QualityMask *m, unsigned key, bool pressed);

// OR of the layer bits of every held key; `layer` has `key_count` entries.
uint8_t quality_mask_get(const QualityMask *m, const uint8_t *layer, unsigned key_count);
