class Note {
public:
    Note(unsigned short);
    bool on();
    unsigned int freq();
    unsigned short volume();
    const char* name();
    unsigned short increment();
    unsigned short decrement();
private:
    unsigned short note;
    unsigned int normalise(unsigned int value);
};
