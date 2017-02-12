#include "EditMachine.h"

#define EM_SCROLL 0
#define EM_SELECT 1
#define EM_MENU 2
#define EM_NOTE 3
#define EM_PLACE 4

#define MENU_NOTE_MAX 88
#define LAST_MENU_ITEM_INDEX 4
static const char* menu[] = { //@TODO: Put into Progmem
    "Move",
    "Copy",
    "Set",
    "Clear",
    "Cancel"
};

EditMachine::EditMachine(Tape *_tape) {
    tape = _tape;
    state = EM_SCROLL;
    menuCursor = 0;
    menuViewportStart = 0;
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
            // if menu clear -> perform and go straight to EM_SCROLL
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
        case EM_NOTE:
        // Optional Note placement menu
        if (mode == CONTROL_CW && menuCursor < LAST_MENU_ITEM_INDEX) menuCursor++;
        if (mode == CONTROL_CCW && menuCursor > 0) menuCursor--;
        break;
    }
}

void EditMachine::displayMenu(char buffer[][21]) {
    size_t viewportSize = 2;
    // Calculate viewport start
    if (menuCursor < menuViewportStart)
        menuViewportStart = menuCursor;
    if (menuCursor > menuViewportStart + (viewportSize -1))
        menuViewportStart = menuCursor - (viewportSize -1);

    // Render current selection
    for (size_t i = 0; i < viewportSize; i++) {
        snprintf(buffer[i], 21, "  %-18s", menu[i+menuViewportStart]);
        bool selected = (i+menuViewportStart == menuCursor);
        if (selected) {
            buffer[i][0] = '>';
        }
    }
}

void EditMachine::displayNoteMenu(char buffer[][21]) {
    snprintf_P(buffer[0], 21, (const char*)F("Note!: %d         "), state);
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
        strncpy_P(buffer[1], (const char*)F("    Select Range    "), 21);
        break;
        case EM_SELECT:
        tape->render(buffer[0]);
        strncpy_P(buffer[1], (const char*)F(" End Selection Range"), 21);
        break;
        case EM_PLACE:
        tape->render(buffer[0]);
        strncpy_P(buffer[1], (const char*)F("    Place action    "), 21);
        break;
        case EM_MENU:
        // Menu is displayed, scroll through it
        this->displayMenu(buffer);
        break;
        case EM_NOTE:
        // Optional Note placement menu
        this->displayNoteMenu(buffer);
        break;
    }

    //snprintf_P(buffer[1], 21, (const char*)F("  RAM free %-4d B"), freeMemory());
}
