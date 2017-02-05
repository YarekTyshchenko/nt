#include "menu.h"

// As a test, lets start with:
// Encoder 1 -> 0
// Edit -> Tape
// Play -> Tape.play()

Menu::Menu() {
    MenuItem items[] = {
        MenuItem("Edit"),
        MenuItem("Play"),
    };
    menu = items;
}

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
    //MenuItem *item = &menu[cursor];
    //item->press();
}
