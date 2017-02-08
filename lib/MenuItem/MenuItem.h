#include <Arduino.h>
typedef bool (*RenderFunction)(char[], size_t);

class MenuItem {
public:
    MenuItem(const char*);
    MenuItem(const char*, RenderFunction);
    void press();
    bool render(char[], size_t);
    const char *name;
private:
    RenderFunction renderCallback;
};
