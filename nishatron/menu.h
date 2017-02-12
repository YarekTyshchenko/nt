#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <MenuItem.h>

class Menu {
public:
    Menu(MenuItem*[], size_t);
    void render(char[][21], size_t size);
    void up();
    void down();
    void press();
    void preselect(size_t position, bool enter);
private:
    volatile uint8_t cursor;
    MenuItem** menu;
    size_t menuRowCount;
    volatile bool enterMenuItem;
    size_t viewportStart;
};
#endif
