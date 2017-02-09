#include <WinstarOLED.h>
#include <LiquidCrystal.h>
#include "Rotary.h"
#include "menu.h"
#include "Tape.h"
#include <NewTone.h>

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

    lcd.reset();
    lcd.begin(16, 2);
    lcd.clear();

    tape = new Tape();

    MenuItem* items[] = {
        new MenuItem("  Edit Tape         ", EditTapeRender), // Should call Tape::render() when pressed
        new MenuItem("  Play Tape         ", PlayTapeRender), // Should call Tape::play() and exit
    };

    menu = new Menu(items, 1);

    attachInterrupt(0, rotate, CHANGE);
    attachInterrupt(1, rotate, CHANGE);
}

// Bool = more output required
bool EditTapeRender(void *_menuItem, char foo[], size_t size) {
    String tapeBuffer = tape->render();
    tapeBuffer.toCharArray(foo, 21);

    // Should exit -> is more output required
    return !tape->shouldExit();
};

bool PlayTapeRender(void *_menuItem, char foo[], size_t size) {
    strcpy(foo, "  Play Tape  Yo     ");
    MenuItem *menuItem = (MenuItem*) _menuItem;
    if (menuItem->pressed) {
        menuItem->pressed = false;
        return false;
    }
    return true;
};

bool state = false;
char buffer[][20] = {
    "xxxxxxxxxxxxxxxxxxx",
    "xxxxxxxxxxxxxxxxxxx"
};
char b0[21] = "                    ";
char b1[21] = "XxxxxxxxxxxxxxxxxxxV";
char b2[42] = "11111111111111111111"
              "22222222222222222222";
void loop() {
    lcd.setCursor(0, 0);
    lcd.print("11111111111111111111");
//    lcd.print(tape.render());
    lcd.setCursor(0, 1);
    // lcd.print("Head: ");
    // size_t position = tape.headPosition();
    // lcd.print(position);
    // lcd.print(" Note: ");
    // lcd.print(tape.noteName(tape.noteAt(position)));
    // lcd.print("        ");
    menu->render(b1, 21);
    lcd.print(b1);

    // menu.render(buffer, sizeof(buffer) / sizeof(char[20]));
    // for (size_t i = 0; i < sizeof(buffer) / sizeof(char[20]); i++) {
    //     lcd.setCursor(0, i);
    //     lcd.print(String(buffer[i])); // Remove this String?
    // }

    // Crude handling of a push button
    if (!state && digitalRead(13)) {
        state = true;
        menu->press();
    } else {
        state = digitalRead(13);
    }
}
