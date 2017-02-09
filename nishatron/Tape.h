#include <Arduino.h>

class Tape {
public:
    Tape();
    void left();
    void right();
    void press();
    // One line render
    String render();
    unsigned int headPosition();
    char noteAt(size_t position);
    String noteName(size_t note);
    void play();

    bool shouldExit();
private:
    bool _shouldExit;
    volatile unsigned short _index;
    volatile bool movingHead;
    unsigned int _headPosition;
    int viewportStart;
    char renderNote(char note);
    void noteIncrementPitch(size_t position);
    void noteDecrementPitch(size_t position);
};
