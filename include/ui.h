#ifndef UI_H
#define UI_H

#include "common.h"
#include "player.h"
#include "playlist.h"
#include "visualizer.h"

class UI {
public:
    UI(int argc, char** argv);
    int run();

private:
    void initCSS();
    void buildWidgets();
    
    // --- Signal Handlers ---
    static void onPlayClicked(GtkButton* btn, gpointer data);
    static void onPauseClicked(GtkButton* btn, gpointer data);
    static void onStopClicked(GtkButton* btn, gpointer data);
    static void onAddClicked(GtkButton* btn, gpointer data);
    static void onClearClicked(GtkButton* btn, gpointer data); // New Clear Handler
    static void onPrevClicked(GtkButton* btn, gpointer data);
    static void onNextClicked(GtkButton* btn, gpointer data);
    
    // Key Press Handler (Keyboard Navigation)
    static gboolean onKeyPress(GtkWidget* widget, GdkEventKey* event, gpointer data);

    // --- Members ---
    AppState appState;
    Player* player;
    PlaylistManager* playlistMgr;
    Visualizer* visualizer;

    // Widgets
    GtkWidget* window;
    GtkWidget* drawingArea; // The Visualizer Canvas
    GtkWidget* playlistBox; // The List
    GtkWidget* lblInfo;     // Status Text
};

#endif
