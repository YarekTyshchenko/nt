#include <MemoryFree.h>
#include "MenuItem.h"
#include "Tape.h"

class EditMachine {
public:
    EditMachine(Tape*);
    void control(uint8_t mode);
    void render(char buffer[][21]);
private:
    Tape *tape;
    uint8_t state;
};
