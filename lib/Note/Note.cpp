#include <Note.h>

// pgm_read_byte(data)
static const unsigned int frequencies[] = {
    31,33,35,37,39,41,44,46,49,52,55,58,62,65,69,73,78,82,87,93,98,104,110,
    117,123,131,139,147,156,165,175,185,196,208,220,233,247,262,277,294,311,
    330,349,370,392,415,440,466,494,523,554,587,622,659,698,740,784,831,880,
    932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,1976,2093,
    2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,4186,4435,4699,4978
};
static const char* notes[] = {
    "B 0","C 1","C#1","D 1","D#1","E 1","F 1","F#1","G 1","G#1","A 1","A#1",
    "B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2",
    "B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3",
    "B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4",
    "B 4","C 5","C#5","D 5","D#5","E 5","F 5","F#5","G 5","G#5","A 5","A#5",
    "B 5","C 6","C#6","D 6","D#6","E 6","F 6","F#6","G 6","G#6","A 6","A#6",
    "B 6","C 7","C#7","D 7","D#7","E 7","F 7","F#7","G 7","G#7","A 7","A#7",
    "B 7","C 8","C#8","D 8","D#8"
};

/**
 * @TODO:
    cousteau> yarekt, if you can do with 85 notes (that's 7 octaves including both C at each end), you could have 3
              levels, and still have space for an extra value to represent silence

 * What is a note:
 *  0   First note
 *  88  Last note

    89  First note half volume
    177 Last note half volume

    178 Note off
    178 Note off half volume

    Control
 *  180 loop 1 bar loop 1 time
 *  181 loop 1 bar loop 2 times
 *  182 loop 1 bar loop 4 times
 *  183 loop 1 bar looping forever
 *  184 loop 2 bars loop 1 time
 *  185 loop 2 bars loop 2 times
 *  186 loop 2 bars loop 4 times
 *  187 loop 2 bars looping forever
 *  188 loop 4 bars loop 1 time
 *  189 loop 4 bars loop 2 times
 *  190 loop 4 bars loop 4 times
 *  191 loop 4 bars looping forever
 *  192 loop from start loop 1 time
 *  193 loop from start loop 2 times
 *  194 loop from start loop 4 times
 *  195 loop from start looping forever
 *  196 end track
 *  197+ Reserved
 */

Note::Note(unsigned char value) {
    note = value;
}
unsigned char Note::id() {
    return note;
}

// @TODO: Refactor note hanlding 1) Delete note 2) modulo 88 + /88 3) Bulk change
unsigned char Note::increment() {
    // Break into component + volume
    unsigned char pitch = this->pitch(note);
    unsigned char volume = this->volume();

    // We are at the edge, remove the note
    if (pitch == 88) {
        // TODO: Implement note off half volume
        note = NOTE_NULL;
        return note;
    }
    // Increase the component
    pitch++;

    note = volume * 89 + pitch;
    return note;
}

unsigned char Note::decrement() {
    // Break into component + volume
    unsigned char pitch = this->pitch(note);
    unsigned char volume = this->volume();

    // We are at the edge, remove the note
    if (pitch == 0) {
        note = NOTE_NULL;
        return note;
    }
    // Decrease the component
    pitch--;

    note = volume * 89 + pitch;
    return note;
}

unsigned int Note::pitch(unsigned int value) {
    if (value <= 88) {
        return value;
    }
    if (value <= 177) {
        return value - 89;
    }
    return 0;
}

bool Note::on() {
    if (note == NOTE_NULL) {
        return false;
    }
    return true;
}

unsigned int Note::freq() {
    unsigned int normalised = this->pitch(note);
    return frequencies[normalised];
}

unsigned char Note::volume() {
    return 0; // 0 full volume, 1 half volume
}

const char* Note::name() {
    if (! this->on()) {
        return "---";
    }
    unsigned int normalised = this->pitch(note);
    return notes[normalised];
}
