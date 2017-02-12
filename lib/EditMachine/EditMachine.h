#include <MemoryFree.h>
#include "MenuItem.h"
#include "Tape.h"

class EditMachine {
public:
    EditMachine(Tape*);
    void control(uint8_t mode);
    void render(char buffer[][21]);
private:
    void displayMenu(char buffer[][21]);
    void displayNoteMenu(char buffer[][21]);
    Tape *tape;
    volatile uint8_t state;
    volatile uint8_t menuCursor;
    uint8_t menuViewportStart;
    volatile size_t selectionStart;
    volatile size_t selectionEnd;
};
