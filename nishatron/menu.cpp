#include "menu.h"

// As a test, lets start with:
// Encoder 1 -> 0
// Edit -> Tape
// Play -> Tape.play()

Menu::Menu() {

}

Menu::Menu(MenuItem _menu[], size_t _size) {
    // Viewport
    // Cursor vertical
    // Cursor level
    // Static menu
    menu = _menu;
    size = _size;
    cursor = 0;
    enterMenuItem = false;
}

// void Menu::render(char buffer[][20], size_t size) {
//     // Loop through each buffer
//     for (size_t i = 0; i < size; i++) {
//         // Render current selection
//         MenuItem *item = &menu[cursor];
//         item->render(buffer[i], 20);
//     }
// }

// @TODO: Convert to render two lines;
void Menu::render(char buffer[], size_t size) {
    // Render current selection
    MenuItem item = menu[cursor];
    if (enterMenuItem) {
        // Enter, should exit? true, get back to rendering the menu item
        enterMenuItem = item.render(buffer, size);
        //enterMenuItem = false;
        return;
    }

    strcpy(buffer, item.name); // Fix buffer sizing issues
    buffer[0] = '>'; // Blank the rest of the row too.
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
    enterMenuItem = true;
    MenuItem item = menu[cursor];
    item.press();
}
