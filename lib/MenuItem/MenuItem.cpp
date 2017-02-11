#include <Arduino.h>
#include "MenuItem.h"
static void defaultCallback(void *_menuItem, char buffer[21], bool selected) {
    MenuItem *it = (MenuItem*) _menuItem;
    snprintf(buffer, 21, "  %-18s", it->name);
    if (selected) {
        buffer[0] = '>';
    }
}

MenuItem::MenuItem(const char *_name, RenderFunction _renderCallback, ControlFunction _controlCallback) {
    name = _name;
    nameCallback = defaultCallback;
    renderCallback = _renderCallback;
    controlCallback = _controlCallback;
    pressed = false;
}

MenuItem::MenuItem(NameFunction _nameCallback, RenderFunction _renderCallback, ControlFunction _controlCallback) {
    nameCallback = _nameCallback;
    renderCallback = _renderCallback;
    controlCallback = _controlCallback;
    pressed = false;
}

void MenuItem::press() {
    pressed = true;
    controlCallback(CONTROL_PRESS);
}

void MenuItem::cw() {
    controlCallback(CONTROL_CW);
}

void MenuItem::ccw() {
    controlCallback(CONTROL_CCW);
}

bool MenuItem::render(char buffer[][21], size_t rows) {
    return this->renderCallback(this, buffer, rows);
}

void MenuItem::renderName(char buffer[21], bool selected) {
    this->nameCallback(this, buffer, selected);
}
