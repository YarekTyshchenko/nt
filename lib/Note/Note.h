#ifndef NOTE_H
#define NOTE_H

#define NOTE_NULL 178
#define NOTE_NULL_HALF_VOLUME 179

class Note {
public:
    Note(unsigned char);
    unsigned char id();
    bool on();
    unsigned int freq();
    unsigned char volume();
    const char* name();
    unsigned char increment();
    unsigned char decrement();
private:
    unsigned char note;
    unsigned int pitch(unsigned int value);
};
#endif
