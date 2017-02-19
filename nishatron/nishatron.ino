#include <MemoryFree.h>
#include <WinstarOLED.h>
#include <LiquidCrystal.h>
#include "Rotary.h"
#include "menu.h"
#include "Tape.h"
#include "EditMachine.h"
#include <NewTone.h>

WinstarOLED lcd;
Rotary rotary = Rotary(2, 3);
Tape *tape;
Menu *menu;
EditMachine *editMachine;

// rotate is called anytime the rotary inputs change state.
void rotate() {
    unsigned char result = rotary.process();
    if (result == DIR_CW) {
        menu->down();
    } else if (result == DIR_CCW) {
        menu->up();
    }
}

void createChars(WinstarOLED lcd) {
    uint8_t char1[] = {0x4, 0x4, 0xe, 0x15, 0x15, 0x15, 0xe, 0x0};
    lcd.createChar(1, char1);
    uint8_t char2[] = {0x4, 0x1f, 0x15, 0x15, 0x15, 0x1f, 0x4, 0x0};
    lcd.createChar(2, char2);
}

void setup() {
    //Serial.begin(9600);

    lcd.reset(100); // Default is 2 seconds
    lcd.begin(16, 2);
    lcd.clear();
    createChars(lcd);

    tape = new Tape();
    editMachine = new EditMachine(tape);

    MenuItem* items[] = {
        // @TODO: Do a trick where if you press twice on the same spot, you exit out of a mode
        //new MenuItem("Bulk Apply", NoopRender, NoopControl),
        // @TODO: Move tape and stuff, start selection -> end selection -> Choose note
        new MenuItem("Edit Tape", EditTapeRender, EditTapeControl),
        new MenuItem("Place Notes", PlaceNotesRender, PlaceNotesControl),
        new MenuItem("Play Tape", PlayTapeRender, PlayTapeControl),
        new MenuItem("Save Tape", SaveTapeRender, NoopControl),
        new MenuItem("Clear Tape", ClearTapeRender, NoopControl),
        //new MenuItem("BPM", NoopRender, NoopControl),
        new MenuItem(MemoryNameRender, NoopRender, NoopControl),
        new MenuItem("Character Set", CharsetRender, CharsetControl),
    };

    menu = new Menu(items, sizeof(items) / sizeof(items[0]));
    menu->preselect(0, true);

    attachInterrupt(0, rotate, CHANGE);
    attachInterrupt(1, rotate, CHANGE);
}
volatile uint8_t charsetCursor = 0;
bool CharsetRender(void *_menuItem, char buffer[][21], size_t rows) {
    MenuItem *item = (MenuItem *) _menuItem;
    if (item->pressed) {
        item->pressed = false;
        return false;
    }
    for (size_t i = 0; i < 2; i++) {
        for (size_t k = 0; k < 20; k++) {
            buffer[i][k] = (char) (charsetCursor + (k));
        }
    }
    snprintf_P(buffer[1], 21, (const char*)F("[%3d]"), charsetCursor);
    return true;
}

void CharsetControl(uint8_t mode) {
    if (mode == CONTROL_CW) {
        charsetCursor++;
    } else if (mode == CONTROL_CCW) {
        charsetCursor--;
    }
}

bool NoopRender(void *_menuItem, char buffer[][21], size_t rows) {
    return false;
}

void NoopControl(uint8_t mode) {

}

// Tape Save and Clear
bool ClearTapeRender(void* _menuItem, char buffer[][21], size_t rows) {
    MenuItem *item = (MenuItem *) _menuItem;
    if (item->pressed) {
        item->pressed = false;
        tape->clearTape();
        return false;
    }

    strncpy_P(buffer[0], (const char*)F("    Click again     "), 21);
    strncpy_P(buffer[1], (const char*)F("   to erase Tape    "), 21);
    return true;
}

bool SaveTapeRender(void* _menuItem, char buffer[][21], size_t rows) {
    MenuItem *item = (MenuItem *) _menuItem;
    if (item->pressed) {
        item->pressed = false;
        tape->saveTape();
        return false;
    }

    strncpy_P(buffer[0], (const char*)F("    Click again     "), 21);
    strncpy_P(buffer[1], (const char*)F("   to save Tape     "), 21);
    return true;
}

// Ram display
void MemoryNameRender(void *_menuItem, char buffer[21], bool selected) {
    snprintf_P(buffer, 21, (const char*)F("  RAM free %-4d B"), freeMemory());
    if (selected) {
        buffer[0] = '>';
    }
}

// Edit Tape
bool tapeShouldExit = false;
bool EditTapeRender(void *_menuItem, char buffer[][21], size_t rows) {
    editMachine->render(buffer);
    return !editMachine->shouldExit();
}

void EditTapeControl(uint8_t mode) {
    //@TODO: Implement hold?
    editMachine->control(mode);
}

// Place Notes
bool PlaceNotesRender(void *_menuItem, char buffer[][21], size_t rows) {
    tape->render(buffer[0]);
    size_t position = tape->headPosition();
    Note* note = tape->noteAt(position);

    if (tape->isEdittingNote()) {
        snprintf_P(buffer[1], 21, (const char*)F("Note: %s F: %d "), note->name(), note->freq());
    } else {
        snprintf_P(buffer[1], 21, (const char*)F("  %s at [%3d] "), note->name(), position);
    }
    delete note;
    //snprintf(buffer[0], 21, "Memory: %d", freeMemory());

    // Should exit -> is more output required
    if (tapeShouldExit) {
        tapeShouldExit = false;
        return false;
    }
    return true;
};

void PlaceNotesControl(uint8_t mode) {
    if ((mode == CONTROL_CCW && tape->atLeftStop()) || (mode == CONTROL_CW && tape->atRightStop())) {
        tapeShouldExit = true;
    }
    if (mode == CONTROL_CW) {
        tape->right();
    } else if (mode == CONTROL_CCW) {
        tape->left();
    } else if (mode == CONTROL_PRESS) {
        tape->press();
    }
}

// Tape playback
bool playing = false;
bool PlayTapeRender(void *_menuItem, char buffer[][21], size_t rows) {
    tape->render(buffer[0]);
    snprintf_P(buffer[1], 21, (const char*)F("Playing: [%d]      "), tape->headPosition());
    if (playing)
        tape->advancePlayhead();
    if (tapeShouldExit) {
        tapeShouldExit = false;
        // Reached the end
        tape->reset();
        return false;
    }
    return true;
};

void PlayTapeControl(uint8_t mode) {
    if (
        (mode == CONTROL_CCW && tape->atLeftStop()) ||
        (mode == CONTROL_CW && tape->atRightStop()) ||
        (playing && tape->atRightStop())
    ) {
        tapeShouldExit = true;
    }
    if (mode == CONTROL_CW) {
        playing = false;
        tape->right();
        tape->playNoteAtCursor(tape->headPosition(), 100);
    } else if (mode == CONTROL_CCW) {
        playing = false;
        tape->left();
        tape->playNoteAtCursor(tape->headPosition(), 100);
    } else if (mode == CONTROL_PRESS) {
        // Reset playback head on first activation
        playing = !playing;
    }
}

bool state = false;
char oledBuffer[][21] = {
    "xxxxxxxxxxxxxxxxxxxx",
    "xxxxxxxxxxxxxxxxxxxx"
};
void loop() {
    size_t rowCount = sizeof(oledBuffer) / sizeof(oledBuffer[0]);
    menu->render(oledBuffer, rowCount);
    for (size_t i = 0; i < rowCount; i++) {
        lcd.setCursor(0, i);
        lcd.print(oledBuffer[i]);
    }

    // Crude handling of a push button
    if (!state && digitalRead(13)) {
        state = true;
        menu->press();
    } else {
        state = digitalRead(13);
    }
}
