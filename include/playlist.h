#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "common.h"
#include "player.h"

class PlaylistManager {
public:
    PlaylistManager(AppState* state, Player* player, GtkWidget* listBox);
    
    void addFiles();
    void clear(); // Added for your "Clear Playlist" request
    void onRowActivated(GtkListBox* box, GtkListBoxRow* row);
    void refreshUI();
    
    // Selection/Keyboard helpers
    void selectNext();
    void selectPrev();
    void deleteSelected();

private:
    AppState* app;
    Player* player;
    GtkWidget* listBox; 
    GtkWidget* parentWindow;
};

#endif
