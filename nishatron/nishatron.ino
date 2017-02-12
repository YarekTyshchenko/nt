#include <WinstarOLED.h>
#include <LiquidCrystal.h>
#include "Rotary.h"
#include "menu.h"
#include "Tape.h"
#include <NewTone.h>
#include <MemoryFree.h>

WinstarOLED lcd;
Rotary rotary = Rotary(2, 3);
Tape *tape;
Menu *menu;

// rotate is called anytime the rotary inputs change state.
void rotate() {
    unsigned char result = rotary.process();
    if (result == DIR_CW) {
        menu->down();
    } else if (result == DIR_CCW) {
        menu->up();
    }
}

void setup() {
    //Serial.begin(9600);

    lcd.reset(100); // Default is 2 seconds
    lcd.begin(16, 2);
    lcd.clear();

    tape = new Tape();

    MenuItem* items[] = {
        new MenuItem("Settings", NoopRender, NoopControl),
        // @TODO: Choose note -> start selection -> end selection
        new MenuItem("Bulk Apply", NoopRender, NoopControl),
        // @TODO: Move tape and stuff
        new MenuItem("Edit Tape", EditTapeRender, NoopControl),
        new MenuItem("Place Notes", EditTapeRender, EditTapeControl),
        new MenuItem("Play Tape", PlayTapeRender, PlayTapeControl),
        new MenuItem("Save Tape", SaveTapeRender, NoopControl),
        new MenuItem("Clear Tape", ClearTapeRender, NoopControl),
        new MenuItem("BPM", NoopRender, NoopControl),
        new MenuItem(MemoryNameRender, NoopRender, NoopControl),
    };

    menu = new Menu(items, sizeof(items) / sizeof(items[0]));
    menu->preselect(1, true);

    attachInterrupt(0, rotate, CHANGE);
    attachInterrupt(1, rotate, CHANGE);
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

// Place Notes
bool EditTapeRender(void *_menuItem, char buffer[][21], size_t rows) {
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
    return !tape->shouldExit();
};

void EditTapeControl(uint8_t mode) {
    if (mode == CONTROL_CW) {
        tape->right();
    } else if (mode == CONTROL_CCW) {
        tape->left();
    } else if (mode == CONTROL_PRESS) {
        tape->press();
    }

    if (!tape->isEdittingNote() && (mode == CONTROL_CW || mode == CONTROL_CCW)) {
        tape->playNoteAtCursor(tape->headPosition(), 100);
    }
}

// Tape playback
bool playing = false;
bool PlayTapeRender(void *_menuItem, char buffer[][21], size_t rows) {
    tape->render(buffer[0]);
    snprintf_P(buffer[1], 21, (const char*)F("Playing: [%d]      "), tape->headPosition());
    if (playing)
        tape->advancePlayhead();
    if (tape->shouldExit()) {
        // Reached the end
        tape->reset();
        return false;
    }
    return true;
};

void PlayTapeControl(uint8_t mode) {
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
