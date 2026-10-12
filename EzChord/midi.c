#include "EzChord/midi.h"
#include "EzChord/config.h"
#include "tusb.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


//static bool playing = false;
static uint8_t msg[3];
static int8_t octave = 2;
static bool bass = 0; 
static int8_t scale_modifier = 0;
static uint8_t notes[MAX_NOTE] = {0};
const char *scale_notes[12] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

const uint8_t scale_interval[] = {0, 2, 4, 5, 7, 9, 11};

static char msg_chord_name[20];

void set_bass(bool val){
    bass = val;
    //printf("Bass value: %u \n",bass);
}

int get_bass(){
    return bass;
}

void set_scale(int scale){
    scale_modifier += scale;
    if (scale_modifier > 11){
        scale_modifier = 0;
    } else if (scale_modifier < 0 && scale == -1){
        scale_modifier = 11;
    }
    //printf("Scale value: %u \n",scale_modifier);

}

int get_scale(){
    return scale_modifier;
}

const char** get_scale_notes(){
    return scale_notes;
}

const uint8_t* get_scale_interval(){
    return scale_interval;
}

void set_octave(int oct){
    octave += oct;
    if (octave < -1){
        octave = -1;
    } else if (octave > 8){
        octave = 8;
    }
    //printf("Octave value: %i \n",octave);

} 

int get_octave(){
    return octave;
}

void get_chord_name(char *dest){
    strcpy(dest, msg_chord_name);
}

void note_off(int note){
    msg[0] = 0x80;                    // Note Off - Channel 1
    msg[1] = note;
    msg[2] = 0;
    tud_midi_n_stream_write(0, 0, msg, 3); 
}

void note_on(int note, int velocity){
    // Then play
    msg[0] = 0x90;                    // Note On - Channel 1
    msg[1] = note;
    msg[2] = velocity;                // Velocity
    tud_midi_n_stream_write(0, 0, msg, 3); 
}

void midi_chord_name(int root, bool minor, bool six, bool add7, bool add9, bool diminished){
    msg_chord_name[0] = '\0';
    int ix = root % 12;
    char chord[2];
    strcpy(chord, scale_notes[ix]);
    strcat(msg_chord_name, chord);
    if (minor){
        strcat(msg_chord_name, "m");
    }else if (diminished){
        strcat(msg_chord_name, "dim");
    }else{
        if (add7 || add9 ){
            strcat(msg_chord_name, "maj");
            if (add7 && add9){
                strcat(msg_chord_name, "9");
            } else if(add9){
                strcat(msg_chord_name, "(add 9)");
            }else{
                strcat(msg_chord_name, "7");
            }
            if(six){
                strcat(msg_chord_name, "/6");
            }
        } 
    }
    //printf("Chord name: %s \n", msg_chord_name);
}

//returns notes based on root note
uint8_t * midi_chord_notes(int root, bool minor, bool six, bool add7, bool add9, bool diminished){
    notes[0] = root;
    notes[1] = root + 4 - (minor || diminished);
    notes[2] = root + 7 - diminished;
    int next = 3;
    if (six){
        notes[next] = root + 9;
        next++;
    }
    if (add7){
        notes[next] = 11 - minor - 2*diminished;
        next++;
    }
    if (add9){
        notes[next] = 14 - diminished;
    }
    return notes;
} 

