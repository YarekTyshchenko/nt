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

// @TODO: Convert to render two lines;
void Menu::render(char buffer[][21], size_t viewportSize) {
    // Handle case when: menu 5, viewportSize: 2, cursor: 4
    // Calculate viewport start
    if (cursor < viewportStart)
        viewportStart = cursor;
    if (cursor > viewportStart + (viewportSize -1))
        viewportStart = cursor - (viewportSize -1);


    for (size_t i = 0; i < viewportSize; i++) {
        // render everything
        MenuItem *it = menu[i+viewportStart];
        strcpy(buffer[i], it->name);
        if (i+viewportStart == cursor) {
            buffer[i][0] = '>'; // Blank the rest of the row too.
        }
    }
    //     // Render current selection
    //     MenuItem* item = menu[cursor];
    //     // Is menu item activated?
    //     if (enterMenuItem) {
    //         // Does this render require more rendering?
    //         enterMenuItem = item->render(buffer, size);
    //         return;
    //     }
    //
    //     strcpy(buffer, item->name); // Fix buffer sizing issues
    // }
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
