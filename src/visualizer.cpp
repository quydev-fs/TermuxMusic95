#include "visualizer.h"
#include <cmath>
#include <cstdlib>

Visualizer::Visualizer(AppState* state) : app(state) {}

gboolean Visualizer::onTick(gpointer widget) {
    gtk_widget_queue_draw(GTK_WIDGET(widget));
    return G_SOURCE_CONTINUE;
}

gboolean Visualizer::onDraw(GtkWidget* widget, cairo_t* cr, gpointer data) {
    AppState* app = (AppState*)data;
    
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    int width = alloc.width;
    int height = alloc.height;

    // 1. Background (Black)
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    // 2. Clip to ensure we never draw outside boundaries (Fixes Overlap Bug)
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_clip(cr);

    if (!app->playing) {
        // Draw flat line if stopped
        cairo_set_source_rgb(cr, 0, 0.88, 0); // Winamp Green
        cairo_set_line_width(cr, 2);
        cairo_move_to(cr, 0, height / 2);
        cairo_line_to(cr, width, height / 2);
        cairo_stroke(cr);
        return FALSE;
    }

    // 3. Draw Fake Visualization (Bars)
    // In a real app, we'd pull FFT data from GStreamer here
    cairo_set_source_rgb(cr, 0, 0.88, 0); // Green
    
    int bars = 16;
    double barWidth = (double)width / bars;
    
    for (int i = 0; i < bars; i++) {
        // Generate random height based on playback state
        double h = (rand() % (height - 5)) + 5;
        
        double x = i * barWidth + 1;
        double y = height - h;
        
        cairo_rectangle(cr, x, y, barWidth - 2, h);
        cairo_fill(cr);
        
        // Draw "Peak" (Yellow)
        cairo_set_source_rgb(cr, 0.8, 0.8, 0);
        cairo_rectangle(cr, x, y - 2, barWidth - 2, 2);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 0, 0.88, 0); // Reset to green
    }

    return FALSE;
}
