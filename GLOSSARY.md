# Glossary

- **Key**: a physical chord or quality switch, identified by its index in a matrix (row * cols + col).
- **Degree**: scale position (I..vii) a chord key plays, from `CHORD_LAYER`.
- **Quality**: bit set (minor, six, add7, add9, dim) that shapes a chord. A chord key carries its own; held quality keys are OR-ed in. Bit layout lives in `quality_bits.h`; held mask is recomputed from held keys (`quality_mask`).
- **Voicing**: the MIDI notes and display name produced for a degree, quality, octave and scale (`chord_voice`).
- **Chord**: the result of a voicing: notes, count, name.
- **Chord player**: turns key edges into note on/off (last press wins) through a MIDI-out adapter (USB in firmware, recording fake in tests); owns the sounding notes and displayed name.
- **Debounce**: turning raw switch samples into one clean pressed/released edge (`debounce_update`).
- **Key matrix**: row/column scanned grid of keys; reports debounced key edges through a pin adapter (GPIO in firmware, scripted fake in tests).
- **Octave**: -1..8 register shift for chords (2 = C2 base); clamps at the ends.
- **Scale**: transposition of the whole keyboard in semitones, 0..11; wraps around.
- **Mode**: whether the encoder currently changes the octave or the scale.
- **Performance**: the live settings (octave, scale, bass, mode) in `performance.h`.
- **Display layout**: pure placement of the chord name, status line and underline for the 128x64 screen; draws through a sink adapter (OLED in firmware, recording fake in tests).
