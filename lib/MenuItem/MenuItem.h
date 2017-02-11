#include <Arduino.h>
typedef bool (*RenderFunction)(void*, char[][21], size_t rows);
typedef void (*ControlFunction)(uint8_t mode);
typedef void (*NameFunction)(void*, char[21], bool selected);

#define CONTROL_CW    0b01
#define CONTROL_CCW   0b10
#define CONTROL_PRESS 0b11

static void defaultCallback(void*, char[21]);

class MenuItem {
public:
    MenuItem(const char*, RenderFunction, ControlFunction);
    MenuItem(NameFunction, RenderFunction, ControlFunction);
    bool render(char buffer[][21], size_t rows);
    void renderName(char buffer[21], bool selected);
    void press();
    void cw();
    void ccw();

    const char *name;
    volatile bool pressed;
private:
    RenderFunction renderCallback;
    ControlFunction controlCallback;
    NameFunction nameCallback;
};
