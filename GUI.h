#ifndef GUI_H
#define GUI_H

#include "AppState.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

class GUI {
public:
    GUI(AppState* state);
    ~GUI();
    bool init();
    void runLoop();

private:
    void render();
    void handleInput(int x, int y, int type);
    void handleKey(KeySym ks);
    
    // Helpers
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
