#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "common.h"

class Visualizer {
public:
    Visualizer(AppState* state);
    
    // GTK Drawing Callback
    static gboolean onDraw(GtkWidget* widget, cairo_t* cr, gpointer data);
    
    // Timer for updates
    static gboolean onTick(gpointer widget);

private:
    AppState* app;
};

#endif
