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
}

void MenuItem::cw() {

}

void MenuItem::ccw() {

}

bool MenuItem::render(char buffer[], size_t size) {
    return this->renderCallback(this, buffer, size);
}
