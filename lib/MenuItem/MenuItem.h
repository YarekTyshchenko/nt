#include <Arduino.h>
typedef bool (*RenderFunction)(void*, char[], size_t);
typedef void (*ControlFunction)(uint8_t mode);

#define CONTROL_CW    0b01
#define CONTROL_CCW   0b10
#define CONTROL_PRESS 0b11

class MenuItem {
public:
    MenuItem(const char*, RenderFunction, ControlFunction);
    bool render(char[], size_t);
    void press();
    void cw();
    void ccw();

    const char *name;
    volatile bool pressed;
private:
    RenderFunction renderCallback;
    ControlFunction controlCallback;
};
