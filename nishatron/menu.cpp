#include "menu.h"

Menu::Menu(MenuItem* _menu[], size_t _size) {
    // Viewport
    // Cursor vertical
    menu = _menu;
    size = _size;
    cursor = 0;
    enterMenuItem = false;
}

// @TODO: Convert to render two lines;
void Menu::render(char buffer[], size_t size) {
    // Render current selection
    MenuItem* item = menu[cursor];
    // Is menu item activated?
    if (enterMenuItem) {
        // Does this render require more rendering?
        enterMenuItem = item->render(buffer, size);
        return;
    }

    strcpy(buffer, item->name); // Fix buffer sizing issues
    buffer[0] = '>'; // Blank the rest of the row too.
}

void Menu::up() {
    if (enterMenuItem) {
        menu[cursor]->ccw();
        return;
    }
    if (cursor > 0)
        cursor--;
}

void Menu::down() {
    if (enterMenuItem) {
        menu[cursor]->cw();
        return;
    }
    if (cursor < size)
        cursor++;
}

void Menu::press() {
    if (enterMenuItem) {
        MenuItem* item = menu[cursor];
        item->press();
        return;
    }
    enterMenuItem = true;
}
