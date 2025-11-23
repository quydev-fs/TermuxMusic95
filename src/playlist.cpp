#include "playlist.h"
#include <iostream>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

PlaylistManager::PlaylistManager(AppState* state, Player* pl, GtkWidget* list) 
    : app(state), player(pl), listBox(list) {
    
    // Find parent window for dialogs
    parentWindow = gtk_widget_get_toplevel(listBox);
}

void PlaylistManager::addFiles() {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new("Add Music",
                                         GTK_WINDOW(parentWindow),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);
    
    // Allow multiple selection
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), TRUE);

    // Filter for Audio
    GtkFileFilter* filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Audio Files");
    gtk_file_filter_add_pattern(filter, "*.mp3");
    gtk_file_filter_add_pattern(filter, "*.wav");
    gtk_file_filter_add_pattern(filter, "*.ogg");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        GSList *filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));
        GSList *iter;
        
        size_t oldSize = app->playlist.size();
        
        for (iter = filenames; iter; iter = iter->next) {
            char *filename = (char *)iter->data;
            app->playlist.push_back(std::string(filename));
            g_free(filename);
        }
        g_slist_free(filenames);
        
        // Update Play Order
        size_t newSize = app->playlist.size();
        app->play_order.resize(newSize);
        // Fill new slots with sequential indices
        for(size_t i = oldSize; i < newSize; i++) {
            app->play_order[i] = i;
        }
        
        // If shuffle is on, we should technically re-shuffle, 
        // but appending is safer for currently playing tracks.

        refreshUI();
    }

    gtk_widget_destroy(dialog);
}

void PlaylistManager::clear() {
    // Stop playback
    player->stop();
    
    // Clear Data
    app->playlist.clear();
    app->play_order.clear();
    app->current_track_idx = -1;
    app->playing = false;
    app->paused = false;
    
    // Refresh UI (Empty List)
    refreshUI();
}

void PlaylistManager::refreshUI() {
    // Clear existing widgets in the listbox
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(listBox));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Add new items
    for (size_t i = 0; i < app->playlist.size(); i++) {
        // We display items in their storage order (0, 1, 2...)
        // even if shuffle plays them in random order (5, 2, 9...)
        std::string path = app->playlist[i];
        
        size_t lastSlash = path.find_last_of("/");
        std::string name = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
        
        std::string labelStr = std::to_string(i + 1) + ". " + name;
        
        GtkWidget* label = gtk_label_new(labelStr.c_str());
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        
        // Highlight if currently playing
        if (app->current_track_idx != -1 && 
            (int)app->play_order[app->current_track_idx] == (int)i) {
            // In GTK, we rely on row selection, but we can color text too
            // This requires CSS or Markup, keeping it simple for now.
        }
        
        gtk_container_add(GTK_CONTAINER(listBox), label);
        gtk_widget_show(label);
    }
}

void PlaylistManager::onRowActivated(GtkListBox* box, GtkListBoxRow* row) {
    int visual_index = gtk_list_box_row_get_index(row);
    if (visual_index >= 0 && visual_index < (int)app->playlist.size()) {
        
        // Find where this visual index lives in the play_order
        // If shuffle is OFF: play_order[i] == i.
        // If shuffle is ON: we need to find 'visual_index' in 'play_order'.
        
        if (!app->shuffle) {
            app->current_track_idx = visual_index;
        } else {
            // In shuffle mode, clicking a song forces it to play,
            // usually resetting the shuffle order or finding it.
            // Simple approach: Find it.
            for(size_t i=0; i<app->play_order.size(); i++) {
                if((int)app->play_order[i] == visual_index) {
                    app->current_track_idx = i;
                    break;
                }
            }
        }

        size_t real_file_index = app->play_order[app->current_track_idx];
        player->load(app->playlist[real_file_index]);
        player->play();
    }
}

void PlaylistManager::selectNext() {
    GtkListBoxRow* row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listBox));
    if (row) {
        int idx = gtk_list_box_row_get_index(row);
        if (idx < (int)app->playlist.size() - 1) {
            GtkListBoxRow* nextRow = gtk_list_box_get_row_at_index(GTK_LIST_BOX(listBox), idx + 1);
            gtk_list_box_select_row(GTK_LIST_BOX(listBox), nextRow);
        }
    }
}

void PlaylistManager::selectPrev() {
    GtkListBoxRow* row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listBox));
    if (row) {
        int idx = gtk_list_box_row_get_index(row);
        if (idx > 0) {
            GtkListBoxRow* prevRow = gtk_list_box_get_row_at_index(GTK_LIST_BOX(listBox), idx - 1);
            gtk_list_box_select_row(GTK_LIST_BOX(listBox), prevRow);
        }
    }
}

void PlaylistManager::deleteSelected() {
     GtkListBoxRow* row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listBox));
     if(row) {
         int idx = gtk_list_box_row_get_index(row);
         app->playlist.erase(app->playlist.begin() + idx);
         // Also need to remove from play_order and rebalance
         // For simplicity in this snippet, we just rebuild play_order linear
         app->play_order.clear();
         app->play_order.resize(app->playlist.size());
         std::iota(app->play_order.begin(), app->play_order.end(), 0);
         
         app->current_track_idx = -1; // Stop playing to avoid crash
         player->stop();
         refreshUI();
     }
}
