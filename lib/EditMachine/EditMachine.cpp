#include "EditMachine.h"

#define EM_SCROLL 0
#define EM_SELECT 1
#define EM_MENU 2
#define EM_NOTE 3
#define EM_PLACE 4

EditMachine::EditMachine(Tape *_tape) {
    tape = _tape;
    state = EM_SCROLL;
}

void EditMachine::control(uint8_t mode) {
    if (mode == CONTROL_PRESS) {
        switch (state) {
            case EM_SCROLL:
            state = EM_SELECT;
            break;
            case EM_SELECT:
            state = EM_MENU;
            break;
            case EM_MENU:
            // if menu selected set note, redirect to EM_NOTE, otherwise:
            case EM_NOTE: // Optional Note placement menu
            state = EM_PLACE;
            break;
            case EM_PLACE:
            state = EM_SCROLL;
            break;
        }
        return;
    }

    switch (state) {
        case EM_SCROLL:
        // Move tape left or right, marks the starting point of selection
        case EM_SELECT:
        // Save the starting point, now we are moving selection
        case EM_PLACE:
        // Placing the final action
        if (mode == CONTROL_CW) tape->right();
        if (mode == CONTROL_CCW) tape->left();
        break;
        case EM_MENU:
        // Menu is displayed, scroll through it
        // if (mode == CONTROL_CW) menu->right();
        // if (mode == CONTROL_CCW) menu->left();
        break;
        case EM_NOTE:
        // Optional Note placement menu
        // if (mode == CONTROL_CW) menu->right();
        // if (mode == CONTROL_CCW) menu->left();
        break;
    }
}

/**
- Scroll -> Drag Selection -> Menu: Move/Copy/Set/Clear/Cancel -> Scroll to placement
                                               \->Select Note-/
*/
void EditMachine::render(char buffer[][21]) {
    // State machine
    switch (state) {
        case EM_SCROLL:
        tape->render(buffer[0]);
        strncpy_P(buffer[1], (const char*)F("   Click to Begin   "), 21);
        break;
        case EM_SELECT:
        tape->render(buffer[0]);
        strncpy_P(buffer[1], (const char*)F("   Select a range   "), 21);
        break;
        case EM_PLACE:
        tape->render(buffer[0]);
        strncpy_P(buffer[1], (const char*)F("    Place action    "), 21);
        break;
        case EM_MENU:
        // Menu is displayed, scroll through it
        snprintf_P(buffer[0], 21, (const char*)F("MENU!: %d         "), state);
        break;
        case EM_NOTE:
        // Optional Note placement menu
        snprintf_P(buffer[0], 21, (const char*)F("Note!: %d         "), state);
        break;
    }

    snprintf_P(buffer[1], 21, (const char*)F("  RAM free %-4d B"), freeMemory());
}
