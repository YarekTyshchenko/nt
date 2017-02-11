#include <Arduino.h>
#include "Note.h"

class Tape {
public:
    Tape();
    void left();
    void right();
    void press();
    // One line render
    void render(char buffer[]);
    size_t headPosition();
    char noteAt(size_t position);
    void playNoteAtCursor(size_t position, unsigned long length);
    const char* noteName(size_t note);
    void play();
    void stop();
    bool isPlaying();
    void reset();
    bool isEdittingNote();

    bool shouldExit();
    volatile size_t viewportStart; // Move this to private
    void advancePlayhead();
private:
    size_t playableLength;
    unsigned long _toneOffAt;
    bool _playback;
    bool _shouldExit;
    volatile bool movingHead;
    volatile size_t _headPosition;
    char renderNote(Note *note);
    void noteIncrementPitch(size_t position);
    void noteDecrementPitch(size_t position);
};
