#include <Arduino.h>
typedef bool (*RenderFunction)(void*, char[], size_t);

class MenuItem {
public:
    MenuItem(const char*, RenderFunction);
    void press();
    bool render(char[], size_t);
    const char *name;
    volatile bool pressed;
private:
    RenderFunction renderCallback;
};
