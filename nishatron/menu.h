#include <Arduino.h>
#include <MenuItem.h>

class Menu {
public:
    Menu();
    Menu(MenuItem[], size_t);
    void render(char[][20], size_t);
    void up();
    void down();
    void press();
private:
    volatile uint8_t cursor;
    MenuItem *menu;
    size_t size;
};
