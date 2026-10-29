#include "EzChord/quality_mask.h"

void quality_mask_set(QualityMask *m, unsigned key, bool pressed)
{
    if (key >= QUALITY_MASK_MAX_KEYS)
        return;
    if (pressed)
        m->held |= (uint8_t)(1u << key);
    else
        m->held &= (uint8_t)~(1u << key);
}

uint8_t quality_mask_get(const QualityMask *m, const uint8_t *layer, unsigned key_count)
{
    uint8_t mask = 0;
    for (unsigned k = 0; k < key_count && k < QUALITY_MASK_MAX_KEYS; k++)
        if (m->held & (1u << k))
            mask |= layer[k];
    return mask;
}
