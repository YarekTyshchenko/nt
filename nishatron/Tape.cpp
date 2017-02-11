#include "Tape.h"
#include <NewTone.h>

#define MAX_TAPE_SIZE 100
#define SCREEN_SIZE 20

char TAPE[MAX_TAPE_SIZE] = {};

const unsigned int frequencies[] = {
    31,33,35,37,39,41,44,46,49,52,55,58,62,65,69,73,78,82,87,93,98,104,110,
    117,123,131,139,147,156,165,175,185,196,208,220,233,247,262,277,294,311,
    330,349,370,392,415,440,466,494,523,554,587,622,659,698,740,784,831,880,
    932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,1976,2093,
    2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,4186,4435,4699,4978
};
const char* notes[] = {
    "B 0","C 1","CS1","D 1","DS1","E 1","F 1","FS1","G 1","GS1","A 1","AS1",
    "B 1","C 2","C#2","D 2","D#2","E 2","F 2","F#2","G 2","G#2","A 2","A#2",
    "B 2","C 3","C#3","D 3","D#3","E 3","F 3","F#3","G 3","G#3","A 3","A#3",
    "B 3","C 4","C#4","D 4","D#4","E 4","F 4","F#4","G 4","G#4","A 4","A#4",
    "B 4","C 5","C#5","D 5","D#5","E 5","F 5","F#5","G 5","G#5","A 5","A#5",
    "B 5","C 6","C#6","D 6","D#6","E 6","F 6","F#6","G 6","G#6","A 6","A#6",
    "B 6","C 7","C#7","D 7","D#7","E 7","F 7","F#7","G 7","G#7","A 7","A#7",
    "B 7","C 8","C#8","D 8","D#8"
};

Tape::Tape() {
    movingHead = true;
    viewportStart = 0; // Left edge of the view port
    _shouldExit = false;
    _headPosition = 0;
}

void Tape::play() {
    for (size_t i = 0; i < sizeof(TAPE) / sizeof(TAPE[0]); i++) {
        char note = TAPE[i];
        // Play a tone
        if (note > 0) {
            unsigned long frequency = frequencies[(size_t)note];
            unsigned long length = 100;
            NewTone(4, frequency, length);
        }
        delay(100);
    }
}

size_t Tape::headPosition() {
    size_t position = _headPosition;
    return position;
}

void Tape::noteIncrementPitch(size_t position) {
    if ((TAPE[position]) < 88)
        TAPE[position]++;
    size_t f = TAPE[position];

    NewTone(4, frequencies[f], 1000);
}

void Tape::noteDecrementPitch(size_t position) {
    if (TAPE[position] > 0)
        TAPE[position]--;
    NewTone(4, frequencies[(size_t)TAPE[position]], 1000);
}

bool Tape::shouldExit() {
    if (_shouldExit) {
        _shouldExit = false;
        return true;
    }
    return false;
}

void Tape::left() {
    if (movingHead) {
        // Exit out of the menu item
        if (_headPosition <= 0) {
            _shouldExit = true;
        }

        if (_headPosition > 0)
            _headPosition--;

        // Hit left stop
        if (_headPosition < viewportStart) {
            viewportStart = _headPosition;
        }

    } else {
        noteDecrementPitch(_headPosition);
    }
}

void Tape::right() {
    if (movingHead) {
        if (_headPosition < MAX_TAPE_SIZE-1)
            _headPosition++;

        // Hit right stop
        if (_headPosition - viewportStart > (SCREEN_SIZE - 1)) {
            viewportStart = _headPosition - (SCREEN_SIZE - 1);
        }
    } else {
        noteIncrementPitch(_headPosition);
    }
}

// Enter / Exit function
void Tape::press() {
    movingHead = !movingHead;
}


char Tape::renderNote(char note) {
    if (note > 0) {
        return 'o';
    }
    return '-';
}

char Tape::noteAt(size_t position) {
    return TAPE[position];
}

const char* Tape::noteName(size_t note) {
    return notes[note];
}

// Render SCREEN_SIZE of TAPE starting from viewportStart
void Tape::render(char screenBuffer[]) {
    // loop for screen width
    for (size_t i = 0; i < SCREEN_SIZE; i++) { // @TODO: Shouldn't that be 21?
        if ((i + viewportStart) % 4 == 0) {
            screenBuffer[i] = '|';
        } else {
            screenBuffer[i] = '-';
        }

        if (i + viewportStart > MAX_TAPE_SIZE-1) {
            screenBuffer[i] = 'X';
            break;
        }
        char note = TAPE[i + viewportStart];
        // display note
        if (note > 0) {
            screenBuffer[i] = renderNote(note);
        }
    }

    // Display Cursor
    // 29 square
    // 255 block
    size_t cursor = _headPosition - viewportStart;
    screenBuffer[cursor] = (char)29;
}
