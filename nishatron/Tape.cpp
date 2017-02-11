#include "Tape.h"
#include <NewTone.h>

#define MAX_TAPE_SIZE 100
#define SCREEN_SIZE 20

unsigned char TAPE[MAX_TAPE_SIZE] = {};

Tape::Tape() {
    // Init the tape to Note 178 -- Off
    memset(TAPE, 178, MAX_TAPE_SIZE);
    movingHead = true;
    viewportStart = 0; // Left edge of the view port
    _shouldExit = false;
    _headPosition = 0;
    _playback = false;
    _toneOffAt = 0;
    playableLength = 0;
}

void Tape::reset() {
    _headPosition = 0;
    viewportStart = 0;
}

void Tape::play() {
    _playback = true;
}

void Tape::playNoteAtCursor(size_t position, unsigned long length) {
    Note note = Note(TAPE[position]);
    if (note.on()) {
        NewTone(4, note.freq(), length);
    }
}

void Tape::advancePlayhead() {
    if (millis() < _toneOffAt) {
        return;
    }
    unsigned long length = 100;
    // Play a tone
    this->playNoteAtCursor(_headPosition, length);
    // Shift tape right
    this->right();
    // Set a future delay
    _toneOffAt = millis() + length;
}

void Tape::stop() {
    _playback = false;
}

bool Tape::isPlaying() {
    return _playback;
}

size_t Tape::headPosition() {
    return _headPosition;
}

void Tape::noteIncrementPitch(size_t position) {
    Note note = Note(TAPE[position]);
    TAPE[position] = note.increment(); //@TODO: Refactor this not to write needlessly

    NewTone(4, note.freq(), 1000);
}

void Tape::noteDecrementPitch(size_t position) {
    Note note = Note(TAPE[position]);
    TAPE[position] = note.decrement();

    NewTone(4, note.freq(), 1000);
}

bool Tape::shouldExit() {
    if (_shouldExit) {
        _shouldExit = false;
        return true;
    }
    return false;
}

bool Tape::isEdittingNote() {
    return !movingHead;
}

// Work out viewport from head position, so playback works
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


Note Tape::noteAt(size_t position) {
    return Note(TAPE[position]);
}

const char* Tape::noteName(size_t note) {
    Note n = Note(note);
    return n.name();
}

char Tape::renderNote(Note *note) {
    if (note->on()) {
        return 'o';
    }
    return '-';
}

// Render SCREEN_SIZE of TAPE starting from viewportStart
void Tape::render(char screenBuffer[]) {
    return;
    // loop for screen width
    for (size_t i = 0; i < SCREEN_SIZE; i++) { // @TODO: Shouldn't that be 21?
        if ((i + viewportStart) % 4 == 0) {
            screenBuffer[i] = '|';
        }

        if (i + viewportStart > MAX_TAPE_SIZE-1) {
            screenBuffer[i] = 'X'; // Protection against display data outside of range
            break;
        }
        Note *note = new Note(TAPE[i + viewportStart]);
        //display note
        if (note->on()) {
           screenBuffer[i] = renderNote(note);
        }
        delete note;
    }

    // Display Cursor
    // 29 square
    // 255 block
    size_t cursor = _headPosition - viewportStart;
    screenBuffer[cursor] = (char)29;
}
