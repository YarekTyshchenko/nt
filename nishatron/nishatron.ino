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
    // Serial.begin(9600);

    lcd.reset();
    lcd.begin(16, 2);
    lcd.clear();

    tape = new Tape();

    MenuItem* items[] = {
        new MenuItem("Settings", NoopRender, NoopControl),
        new MenuItem("BPM", NoopRender, NoopControl),
        new MenuItem("Edit Tape", EditTapeRender, EditTapeControl),
        new MenuItem("Play Tape", PlayTapeRender, PlayTapeControl),
        new MenuItem(MemoryNameRender, NoopRender, NoopControl),
        new MenuItem("Dummy 2", NoopRender, NoopControl),
        new MenuItem("Dummy 3", NoopRender, NoopControl),
        new MenuItem("Dummy 4", NoopRender, NoopControl),
        new MenuItem("Dummy 5", NoopRender, NoopControl),
        new MenuItem("Dummy 6", NoopRender, NoopControl),
    };

    menu = new Menu(items, sizeof(items) / sizeof(items[0]));
    menu->preselect(2, true);

    attachInterrupt(0, rotate, CHANGE);
    attachInterrupt(1, rotate, CHANGE);
}

bool NoopRender(void *_menuItem, char buffer[][21], size_t rows) {
    return false;
}

void MemoryNameRender(void *_menuItem, char buffer[21], bool selected) {
    MenuItem *menuItem = (MenuItem*) _menuItem;
    snprintf(buffer, 21, "  Free ram: %-6d", freeMemory());
    if (selected) {
        buffer[0] = '>';
    }
}

void NoopControl(uint8_t mode) {

}

// Bool = more output required
bool EditTapeRender(void *_menuItem, char buffer[][21], size_t rows) {
    tape->render(buffer[0]);
    size_t position = tape->headPosition();
    const char *note = tape->noteName(tape->noteAt(position));
    //snprintf(buffer[1], 21, "Head: %d Note: %s ", position, note);
    snprintf(buffer[1], 21, "Memory: %d          ", freeMemory());

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
    } else {
        // Error ?
    }

}

bool PlayTapeRender(void *_menuItem, char buffer[][21], size_t rows) {
    // @TODO: Render the tape playing
    // tape->startPlayback()
    // return !tape->isPlaybackFinished();

    MenuItem *menuItem = (MenuItem*) _menuItem;
    if (menuItem->pressed) { //@TODO: This can be refactored into a control callback
        menuItem->pressed = false;
        return false;
    }
    return true;
};

void PlayTapeControl(uint8_t mode) {
    if (mode == CONTROL_PRESS) {
        //tape->play();
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
