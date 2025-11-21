#ifndef UI_H
#define UI_H

#include "common.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

class UI {
public:
    UI(AppState* state);
    ~UI();
    bool init();
    void runLoop();

private:
    void render();
    void handleInput(int x, int y);
    void handleKey(KeySym ks);
    
    void drawBevel(int x, int y, int w, int h, bool sunken);
    void drawButton(int x, int y, int w, int h, const char* label, bool pressed);
    void drawText(int x, int y, const char* str, unsigned long color);

    AppState* app;
    Display* dpy;
    Window win;
    GC gc;
    Atom wmDeleteMessage;
};

#endif
