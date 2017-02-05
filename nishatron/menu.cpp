#include "menu.h"

// As a test, lets start with:
// Encoder 1 -> 0
// Edit -> Tape
// Play -> Tape.play()

Menu::Menu(MenuItem _menu[], size_t _size) {
    // Viewport
    // Cursor vertical
    // Cursor level
    // Static menu
    menu = _menu;
    size = _size;
    cursor = 0;
}

void Menu::render(char buffer[][20], size_t size) {
    // Loop through each buffer
    for (size_t i = 0; i < size; i++) {
        // Render current selection
        MenuItem *item = &menu[cursor];
        item->render(buffer[i], 20);
    }
    // Define visible area 16x2
    // Grab the index
    //unsigned int _index = index;
    //char buffer[20]; // sprintf buffer
    //sprintf(buffer[0], "%s %d %d","menu[0]", _index, millis());
    //return String(buffer);
  //return ( + _index) + " " + String(millis());
}

void Menu::up() {
    if (cursor > 0)
        cursor--;
}

void Menu::down() {
    if (cursor < size)
        cursor++;
}

void Menu::press() {
    //press = !press;
    MenuItem *item = &menu[cursor];
    item->press();
}
