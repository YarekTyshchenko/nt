#include "Note.cpp"

#include <stdio.h>
#include <assert.h>
#include <string.h>

void testNote() {
    // Firts note
    Note* n = new Note(0);
    assert(n->freq() == 31);
    assert(strcmp(n->name(), "B 0") == 0);

    // Last note
    n = new Note(88);
    assert(n->freq() == 4978);
    assert(strcmp(n->name(), "D#8") == 0);

    // Half volume note
    n = new Note(89);
    assert(n->freq() == 31);
    assert(strcmp(n->name(), "B 0") == 0);
    n = new Note(177);
    assert(n->freq() == 4978);
    assert(strcmp(n->name(), "D#8") == 0);

    // No note
    n = new Note(178);
    assert(n->freq() == 31);
    assert(strcmp(n->name(), "---") == 0);
}

void testNotePitch() {
    // Decrement from 0
    Note *n = new Note(0);
    unsigned char d = n->decrement();
    assert(d == 0);
    assert(n->freq() == 31);

    // Decrement from 89
    n = new Note(89);
    d = n->decrement();
    assert(d == 89);
    assert(n->freq() == 31);

    // Increment from 177
    n = new Note(177);
    d = n->increment();
    assert(d == 177);
    assert(n->freq() == 4978);

    // Increment from 88
    n = new Note(88);
    d = n->increment();
    assert(d == 88);
    assert(n->freq() == 4978);
}

void testNoteOff() {
    Note *n = new Note(178);
    assert(n->on() == false);
    n = new Note(60);
    assert(n->on() == true);
}

void foo(Note notes[]) {
    printf("%p\n", (void *)notes);
    printf("%p\n", (void *)notes);
}

void testArrayOfObjects() {
    Note notes[] = {
        Note(0)
    };
    foo(notes);
}

int main(int argc, char const *argv[]) {
    /* code */
    printf("Running test framework\n");
    testNote();
    testNotePitch();
    testNoteOff();
    testArrayOfObjects();
    return 0;
}
