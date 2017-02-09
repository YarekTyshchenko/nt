#include <Arduino.h>

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
    const char* noteName(size_t note);
    void play();

    bool shouldExit();
    volatile size_t viewportStart; // Move this to private
private:
    bool _shouldExit;
    volatile bool movingHead;
    volatile size_t _headPosition;
    char renderNote(char note);
    void noteIncrementPitch(size_t position);
    void noteDecrementPitch(size_t position);
};
