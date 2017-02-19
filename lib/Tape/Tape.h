#ifndef TAPE_H
#define TAPE_H

#include <Arduino.h>
#include "Note.h"

#define MAX_TAPE_SIZE 200

class Tape {
public:
    Tape();
    void left();
    void right();
    void press();
    // One line render
    void render(char buffer[]);
    size_t headPosition();
    bool atRightStop();
    bool atLeftStop();
    Note* noteAt(size_t position);
    void playNote(unsigned char note, unsigned long length);
    void placeNoteAt(size_t position, unsigned char noteId);
    void playNoteAtCursor(size_t position, unsigned long length);
    void play();
    void stop();
    bool isPlaying();
    void reset();
    bool isEdittingNote();

    void clearTape();
    void saveTape();

    volatile size_t viewportStart; // Move this to private
    void advancePlayhead();
private:
    size_t playableLength;
    unsigned long _toneOffAt;
    bool _playback;
    volatile bool movingHead;
    volatile size_t _headPosition;
    char renderNote(Note *note);
    void noteIncrementPitch(size_t position);
    void noteDecrementPitch(size_t position);
};
#endif
