#include "EzChord/chord_voicing.h"
#include "EzChord/chord_player.h"
#include "EzChord/display_layout.h"
#include "EzChord/debounce.h"
#include "EzChord/key_matrix.h"
#include "EzChord/performance.h"
#include "EzChord/quality_mask.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// ---- chord_voice ------------------------------------------------------

static void test_voicing(void)
{
    // Key 4: degree I, plain major. C2 base at octave 2.
    Chord c = chord_voice(4, 0, 2, 0);
    assert(c.count == 3);
    assert(c.notes[0] == 36 && c.notes[1] == 40 && c.notes[2] == 43);
    assert(strcmp(c.name, "C") == 0);

    // Held qualities are OR-ed in, extensions are root-relative.
    c = chord_voice(4, QUALITY_ADD7, 2, 0);
    assert(c.count == 4 && c.notes[3] == 36 + 11);
    assert(strcmp(c.name, "Cmaj7") == 0);

    c = chord_voice(4, QUALITY_MINOR | QUALITY_ADD7 | QUALITY_ADD9, 2, 0);
    assert(c.count == 5);
    assert(c.notes[1] == 39 && c.notes[3] == 36 + 10 && c.notes[4] == 36 + 14);
    assert(strcmp(c.name, "Cm9") == 0);

    c = chord_voice(4, QUALITY_ADD7 | QUALITY_SIX, 2, 0);
    assert(c.count == 5 && c.notes[3] == 36 + 9);
    assert(strcmp(c.name, "Cmaj7/6") == 0);

    c = chord_voice(4, QUALITY_DIM, 2, 0);
    assert(c.notes[1] == 39 && c.notes[2] == 42);
    assert(strcmp(c.name, "Cdim") == 0);

    // Scale transposes, octave shifts, sharp names.
    c = chord_voice(4, 0, 3, 1);
    assert(c.notes[0] == 36 + 12 + 1);
    assert(strcmp(c.name, "C#") == 0);

    // Notes beyond MIDI 127 are dropped, never wrapped.
    c = chord_voice(4, QUALITY_ADD9, 8, 11);
    for (int i = 0; i < c.count; i++)
        assert(c.notes[i] <= 127);
    assert(c.count < 4);

    // Lowest octave stays non-negative.
    c = chord_voice(4, 0, -1, 0);
    assert(c.count == 3 && c.notes[0] == 0);
}

// ---- debounce ----------------------------------------------------------

static void test_debounce(void)
{
    Debounce d = {0};
    for (int i = 0; i < 3; i++)
        assert(debounce_update(&d, true, 4) == DEBOUNCE_NONE);
    assert(debounce_update(&d, true, 4) == DEBOUNCE_PRESSED);
    assert(debounce_update(&d, true, 4) == DEBOUNCE_NONE);

    // A bounce is absorbed, then release is reported once.
    assert(debounce_update(&d, false, 4) == DEBOUNCE_NONE);
    assert(debounce_update(&d, true, 4) == DEBOUNCE_NONE);
    int edges = 0;
    for (int i = 0; i < 10; i++)
        if (debounce_update(&d, false, 4) == DEBOUNCE_RELEASED)
            edges++;
    assert(edges == 1);

    // Noise that never saturates reports nothing.
    Debounce n = {0};
    for (int i = 0; i < 100; i++)
        assert(debounce_update(&n, i % 2, 4) == DEBOUNCE_NONE);
}

// ---- key_matrix with scripted pins ------------------------------------

#define ROWS 2
#define COLS 3
static bool closed[ROWS][COLS];
static int driven_col = -1;
static int events[16][2];
static int event_count;

static void fake_set_column(void *ctx, unsigned col, bool level)
{
    (void)ctx;
    assert(level || driven_col == -1);  // one column low at a time
    driven_col = level ? -1 : (int)col;
}

static bool fake_read_row(void *ctx, unsigned row)
{
    (void)ctx;
    return !(driven_col >= 0 && closed[row][driven_col]);
}

static void fake_settle(void *ctx) { (void)ctx; }

static void record(unsigned key, bool pressed)
{
    events[event_count][0] = (int)key;
    events[event_count][1] = pressed;
    event_count++;
}

static void test_matrix(void)
{
    const KeyMatrixPins pins = {0, fake_set_column, fake_read_row, fake_settle};
    KeyMatrix m;
    key_matrix_init(&m, &pins, ROWS, COLS, record);

    closed[1][2] = true;  // key = 1*3+2 = 5
    for (int i = 0; i < 20; i++)
        key_matrix_tick(&m);
    assert(event_count == 1 && events[0][0] == 5 && events[0][1] == 1);

    closed[1][2] = false;
    for (int i = 0; i < 20; i++)
        key_matrix_tick(&m);
    assert(event_count == 2 && events[1][0] == 5 && events[1][1] == 0);
    assert(driven_col == -1);
}

// ---- chord_player with a recording MIDI out ---------------------------

static char log_buf[256];

static void rec_on(void *ctx, int note, int vel)
{
    (void)ctx;
    sprintf(log_buf + strlen(log_buf), "+%d@%d ", note, vel);
}

static void rec_off(void *ctx, int note)
{
    (void)ctx;
    sprintf(log_buf + strlen(log_buf), "-%d ", note);
}

static void test_player(void)
{
    ChordPlayer p;
    chord_player_init(&p, (MidiOut){0, rec_on, rec_off}, 100);
    ChordContext ctx = {0, 2, 0};
    assert(chord_player_name(&p) == NULL);

    // Press: notes on, name shown.
    log_buf[0] = 0;
    chord_player_key(&p, 4, true, ctx);
    assert(strcmp(log_buf, "+36@100 +40@100 +43@100 ") == 0);
    assert(strcmp(chord_player_name(&p), "C") == 0);

    // Second press replaces: old notes off first, then new ones on.
    log_buf[0] = 0;
    ChordContext minor = {QUALITY_MINOR, 2, 0};
    chord_player_key(&p, 5, true, minor);
    assert(strncmp(log_buf, "-36 -40 -43 +", 13) == 0);

    // Releasing an earlier key does nothing.
    log_buf[0] = 0;
    chord_player_key(&p, 4, false, ctx);
    assert(log_buf[0] == 0 && chord_player_name(&p) != NULL);

    // Releasing the last pressed key silences, using the notes that sounded
    // (not the context passed on release).
    chord_player_key(&p, 5, false, (ChordContext){QUALITY_ADD7, 5, 3});
    assert(strstr(log_buf, "-") != NULL && chord_player_name(&p) == NULL);

    // Out-of-range notes are never sent.
    log_buf[0] = 0;
    chord_player_key(&p, 4, true, (ChordContext){QUALITY_ADD9, 8, 11});
    assert(strstr(log_buf, "+133") == NULL);
}

// ---- display_layout with a recording sink -----------------------------

static char draw_log[512];

static void rec_text(void *ctx, int x, int y, DisplayFont font, const char *text)
{
    (void)ctx;
    sprintf(draw_log + strlen(draw_log), "T%c(%d,%d,%s) ",
            font == DISPLAY_FONT_BIG ? 'B' : 's', x, y, text);
}

static void rec_line(void *ctx, int x0, int y0, int x1, int y1)
{
    (void)ctx;
    sprintf(draw_log + strlen(draw_log), "L(%d,%d,%d,%d) ", x0, y0, x1, y1);
}

static void test_display(void)
{
    const DisplaySink sink = {0, rec_text, rec_line};
    DisplayModel m = {NULL, false, true, 2, "C"};

    // No chord: no big text. Octave mode underlines the octave field.
    draw_log[0] = 0;
    display_render(&m, &sink);
    assert(strstr(draw_log, "TB") == NULL);
    assert(strstr(draw_log, "Ts(2,48,BASS OFF)"));
    assert(strstr(draw_log, "Ts(63,48,OCT +2  C)"));
    assert(strstr(draw_log, "L(63,63,102,63)"));

    // Scale mode moves the underline; a chord is centered (1 char = 13 px).
    m.chord_name = "C";
    m.octave_mode = false;
    m.bass = true;
    draw_log[0] = 0;
    display_render(&m, &sink);
    assert(strstr(draw_log, "TB(57,5,C)"));
    assert(strstr(draw_log, "BASS ON"));
    assert(strstr(draw_log, "L(110,63,124,63)"));

    // Longest status text, negative octave: fits without truncation.
    m.octave = -1;
    m.scale_name = "C#";
    draw_log[0] = 0;
    display_render(&m, &sink);
    assert(strstr(draw_log, "OCT -1  C#)"));

    // A chord name wider than the screen never starts off-screen.
    m.chord_name = "Cmaj7/6add9xx";
    draw_log[0] = 0;
    display_render(&m, &sink);
    assert(strstr(draw_log, "TB(0,5,"));
}

// ---- performance -------------------------------------------------------

static void test_performance(void)
{
    Performance p;
    performance_init(&p);
    assert(p.octave == 2 && p.scale == 0 && !p.bass);

    // Octave mode: turn clamps at both ends.
    for (int i = 0; i < 20; i++)
        performance_turn(&p, 1);
    assert(p.octave == OCTAVE_MAX);
    for (int i = 0; i < 20; i++)
        performance_turn(&p, -1);
    assert(p.octave == OCTAVE_MIN);
    assert(p.scale == 0);  // untouched in octave mode

    // Scale mode: turn wraps in both directions.
    performance_toggle_mode(&p);
    performance_turn(&p, -1);
    assert(p.scale == 11);
    performance_turn(&p, 1);
    assert(p.scale == 0);
    for (int i = 0; i < 12; i++)
        performance_turn(&p, 1);
    assert(p.scale == 0);
    assert(p.octave == OCTAVE_MIN);

    performance_toggle_mode(&p);
    assert(p.mode == PERFORMANCE_MODE_OCTAVE);

    performance_set_bass(&p, true);
    assert(p.bass);

    // Shared pitch names back both the OLED and chord names.
    assert(strcmp(chord_pitch_name(1), "C#") == 0);
}

// ---- quality bits / mask ----------------------------------------------

static void test_quality(void)
{
    // CHORD_KEY round-trips degree and quality.
    const uint8_t e = CHORD_KEY(6, QUALITY_DIM);
    assert(CHORD_KEY_DEGREE(e) == 6);
    assert(CHORD_KEY_QUALITY(e) == QUALITY_DIM);
    assert(CHORD_KEY(1, QUALITY_MINOR) == 0b00100001);

    // Two keys share SIX: releasing one must keep the bit.
    const uint8_t layer[3] = {QUALITY_SIX, QUALITY_SIX | QUALITY_ADD7, QUALITY_ADD9};
    QualityMask m = {0};
    quality_mask_set(&m, 0, true);
    quality_mask_set(&m, 1, true);
    assert(quality_mask_get(&m, layer, 3) == (QUALITY_SIX | QUALITY_ADD7));
    quality_mask_set(&m, 1, false);
    assert(quality_mask_get(&m, layer, 3) == QUALITY_SIX);
    quality_mask_set(&m, 0, false);
    assert(quality_mask_get(&m, layer, 3) == 0);
}

int main(void)
{
    test_quality();
    test_voicing();
    test_debounce();
    test_matrix();
    test_player();
    test_display();
    test_performance();
    puts("ok");
    return 0;
}
