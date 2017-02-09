#include <Arduino.h>
#include "MenuItem.h"

MenuItem::MenuItem(const char *_name, RenderFunction _renderCallback, ControlFunction _controlCallback) {
    name = _name;
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
