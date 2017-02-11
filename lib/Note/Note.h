#define NOTE_NULL 178

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
    unsigned int normalise(unsigned int value);
};
