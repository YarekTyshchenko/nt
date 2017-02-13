#include "EditMachine.h"

#define EM_SCROLL 0
#define EM_SELECT 1
#define EM_MENU 2
#define EM_NOTE 3
#define EM_PLACE 4

#define MENU_NOTE_MAX 88
#define LAST_MENU_ITEM_INDEX 4

#define MENU_MOVE   0
#define MENU_COPY   1
#define MENU_SET    2
#define MENU_CLEAR  3
#define MENU_CANCEL 4

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

    selectionStart = 0;
    selectionEnd = 0;

    noteMenuCursor = 0;
    noteMenuViewportStart = 0;
}

bool EditMachine::shouldExit() {
    if (state != EM_SCROLL) {
        return false;
    }
    if (tape->shouldExit()) {
        selectionStart = 0;
        selectionEnd = 0;
        return true;
    }
    return false;
}

void EditMachine::control(uint8_t mode) {
    if (mode == CONTROL_PRESS) {
        switch (state) {
            case EM_SCROLL: // 0
            state = EM_SELECT;
            selectionStart = tape->headPosition();
            break;
            case EM_SELECT: // 1
            state = EM_MENU;
            selectionEnd = tape->headPosition();
            // @TODO: Normalise start and end!
            menuCursor = 0;
            menuViewportStart = 0;
            break;
            case EM_MENU: // 2
            // if menu selected set note, redirect to EM_NOTE
            if (menuCursor == MENU_SET) {
                state = EM_NOTE;
            // if menu clear -> perform and go straight to EM_SCROLL
            } else if (menuCursor == MENU_CLEAR) {
                for (size_t i = selectionStart; i <= selectionEnd; i++) {
                    tape->placeNoteAt(i, NOTE_NULL);
                }
                state = EM_SCROLL;
            } else if (menuCursor == MENU_CANCEL) {
                state = EM_SCROLL;
                // Reset selection;
                selectionStart = selectionEnd = 0;
            }
            break;
            case EM_NOTE: // 3 Optional Note placement menu
            // Select the note;
            for (size_t i = selectionStart; i <= selectionEnd; i++) {
                tape->placeNoteAt(i, noteMenuCursor);
            }
            // Replace selections with notes
            state = EM_SCROLL;
            break;
            case EM_PLACE: // 4 Only for Copy and Move
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
        if (mode == CONTROL_CW && menuCursor < LAST_MENU_ITEM_INDEX) menuCursor++;
        if (mode == CONTROL_CCW && menuCursor > 0) menuCursor--;
        break;
        case EM_NOTE:
        // Optional Note placement menu
        if (mode == CONTROL_CW && noteMenuCursor < 88) noteMenuCursor++;
        if (mode == CONTROL_CCW && noteMenuCursor > 0) noteMenuCursor--;
        tape->playNote(noteMenuCursor, 50);
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
    size_t viewportSize = 2;
    // Calculate viewport start
    if (noteMenuCursor < noteMenuViewportStart)
        noteMenuViewportStart = noteMenuCursor;
    if (noteMenuCursor > noteMenuViewportStart + (viewportSize -1))
        noteMenuViewportStart = noteMenuCursor - (viewportSize -1);

    // Render current selection
    for (size_t i = 0; i < viewportSize; i++) {
        Note *note = new Note(i+noteMenuViewportStart);
        snprintf(buffer[i], 21, "  Note %s", note->name());
        delete note;
        bool selected = (i+noteMenuViewportStart == noteMenuCursor);
        if (selected) {
            buffer[i][0] = '>';
        }
    }
}

/**
- Scroll -> Drag Selection -> Menu: Move/Copy/Set/Clear/Cancel -> Scroll to placement
                                               \->Select Note-/
*/
void EditMachine::render(char buffer[][21]) {
    // State machine
    size_t viewportStart = tape->viewportStart;
    size_t c1 = (selectionStart - viewportStart);
    size_t c2 = (tape->headPosition() - viewportStart);
    if (selectionStart < viewportStart) {
        c1 = 0;
    } else if (c1 > 20) {
        c1 = 20;
    }
    if (c1 > c2) {
        // Swap
        size_t c3 = c1;
        c1 = c2;
        c2 = c3;
    }
    switch (state) {
        case EM_SCROLL:
        tape->render(buffer[0]);
        snprintf_P(
            buffer[1], 21, (const char*)F("  %s at [%3d/%3d] "),
            tape->noteAt(tape->headPosition())->name(), tape->headPosition(), MAX_TAPE_SIZE-1
        );
        break;
        case EM_SELECT:
        tape->render(buffer[0]);
        this->renderSnake(buffer[1], c1, c2, selectionStart > viewportStart);
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
    //sprintf(buffer[1], "S: %-1d M: %-1d N:%-2d", state, menuCursor, noteMenuCursor);
    //snprintf_P(buffer[1], 21, (const char*)F("  RAM free %-4d B"), freeMemory());
}

void EditMachine::renderSnake(char buffer[21], size_t c1, size_t c2, bool foo) {
    memset(buffer, ' ', 20);
    for (size_t i = c1; i < c2; i++) {
        buffer[i] = '-';
    }
    buffer[c2] = '^';
    if (foo) {
        buffer[c1] = '^';
    }
    // Render counter [ 21/299] left, right, or middle of the screen
    // [ 21/299] ^-[ 21/299]-^
    size_t width = c2 - c1;
    char b2[10];
    sprintf(b2, "[%3d/%3d]", tape->headPosition(), MAX_TAPE_SIZE-1);
    if (width > 9) {
        size_t offset = width/2 - 9/2;
        memcpy(&buffer[c1+offset], b2, 9);
    } else {
        // Try left or right
        if (c1 > 9) {
            // Enough room on the left
            memcpy(buffer, b2, 9);
        } else if (20 - c2 > 9) {
            memcpy(&buffer[20 - 9], b2, 9);
        }
    }
}
