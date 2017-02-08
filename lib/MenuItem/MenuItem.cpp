#include <Arduino.h>
#include "MenuItem.h"

MenuItem::MenuItem(const char *_name) {
    name = _name;
}

MenuItem::MenuItem(const char *_name, RenderFunction _renderCallback) {
    name = _name;
    renderCallback = _renderCallback;
}

void MenuItem::press() {

}

void MenuItem::render(char buffer[], size_t size) {
    this->renderCallback(buffer, size);
}
