#include "EzChord/chord_player.h"
#include <string.h>

void chord_player_init(ChordPlayer *p, MidiOut out, int velocity)
{
    memset(p, 0, sizeof *p);
    p->out = out;
    p->velocity = velocity;
    p->last_pressed = -1;
}

// Switch off exactly the notes that were switched on
static void release_playing(ChordPlayer *p)
{
    for (int i = 0; i < p->playing_count; i++)
        p->out.note_off(p->out.ctx, p->playing[i]);
    p->playing_count = 0;
}

void chord_player_key(ChordPlayer *p, unsigned key, bool pressed, ChordContext ctx)
{
    if (!pressed) {
        if (p->last_pressed == (int)key) {
            p->showing = false;
            release_playing(p);
        }
        return;
    }

    // Replace whatever is sounding, then voice the new chord
    release_playing(p);
    Chord chord = chord_voice(key, ctx.quality, ctx.octave, ctx.scale);
    for (int i = 0; i < chord.count; i++) {
        p->out.note_on(p->out.ctx, chord.notes[i], p->velocity);
        p->playing[i] = chord.notes[i];
    }
    p->playing_count = chord.count;
    strcpy(p->name, chord.name);
    p->showing = true;
    p->last_pressed = (int)key;
}

const char *chord_player_name(const ChordPlayer *p)
{
    return p->showing ? p->name : NULL;
}
