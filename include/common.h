#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <gtk/gtk.h>

// UI Colors (Dark Theme)
#define WINAMP_BG_COLOR "#282828"
#define WINAMP_BTN_COLOR "#333333"
#define WINAMP_FG_COLOR "#00FF00" // Neon green

// Repeat Modes
enum RepeatMode {
    REP_OFF = 0,
    REP_ONE = 1,
    REP_ALL = 2
};

// Application State
struct AppState {
    // Playback
    bool playing = false;
    bool paused = false;
    
    // Playlist
    std::vector<std::string> playlist;
    std::vector<size_t> play_order; // Handles shuffling
    int current_track_idx = -1;
    
    // Modes
    RepeatMode repeatMode = REP_OFF;
    bool shuffle = false;
    
    // --- NEW: MINI MODE FLAG ---
    bool mini_mode = false;
};

#endif
