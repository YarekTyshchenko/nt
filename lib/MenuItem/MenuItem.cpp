#include <Arduino.h>
#include "MenuItem.h"

MenuItem::MenuItem(const char *_name, RenderFunction _renderCallback) {
    name = _name;
    renderCallback = _renderCallback;
    pressed = false;
}

void MenuItem::press() {
    pressed = true;
}

bool MenuItem::render(char buffer[], size_t size) {
    return this->renderCallback(this, buffer, size);
}
