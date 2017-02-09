#include "menu.h"

Menu::Menu(MenuItem* _menu[], size_t _size) {
    // Viewport in Rows
    viewportOffset = 0;
    viewportStart = 0;
    // Cursor vertical
    menu = _menu;
    menuRowCount = _size;
    cursor = 0;
    enterMenuItem = false;
}

void Menu::preselect(size_t position, bool enter) {
    cursor = position;
    enterMenuItem = enter;
}

void Menu::render(char buffer[][21], size_t viewportSize) {
    // Render current selection
    MenuItem* item = menu[cursor];
    // Is menu item activated?
    if (enterMenuItem) {
        // Does this render require more rendering?
        enterMenuItem = item->render(buffer, sizeof(buffer) / sizeof(buffer[0]));
        return;
    }

    // Calculate viewport start
    if (cursor < viewportStart)
        viewportStart = cursor;
    if (cursor > viewportStart + (viewportSize -1))
        viewportStart = cursor - (viewportSize -1);
    // Render selections
    for (size_t i = 0; i < viewportSize; i++) {
        // render everything
        MenuItem *it = menu[i+viewportStart];
        strcpy(buffer[i], it->name);
        if (i+viewportStart == cursor) {
            buffer[i][0] = '>'; // Blank the rest of the row too.
        }
    }
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
    if (cursor < menuRowCount-1)
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
