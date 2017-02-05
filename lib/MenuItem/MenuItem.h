#include <Arduino.h>
typedef void (*RenderFunction)(char[], size_t);

class MenuItem {
public:
    MenuItem(const char*);
    MenuItem(const char*, RenderFunction);
    void press();
    void render(char[], size_t);
private:
    const char *name;
};
