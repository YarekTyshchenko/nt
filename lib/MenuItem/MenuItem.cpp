#include <Arduino.h>
#include "MenuItem.h"

MenuItem::MenuItem(const char *_name) {
    name = _name;
}

MenuItem::MenuItem(const char *_name, RenderFunction) {
    name = _name;
}

void MenuItem::press() {

}

void MenuItem::render(char buffer[], size_t size) {
    strncpy(buffer, name, sizeof(name) / sizeof(char));
}
