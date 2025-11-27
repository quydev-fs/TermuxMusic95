#ifndef CONFIG_H
#define CONFIG_H

// UI Configuration
#define UI_WINDOW_WIDTH 180        // Reduced from 320
#define UI_WINDOW_HEIGHT_INIT 180  // Reduced from 360
#define UI_VISUALIZER_HEIGHT 30    // Reduced from 40
#define UI_MINI_MODE_HEIGHT 135    // Reduced from 180

// UI Element Sizing
#define UI_BUTTON_HEIGHT 18        // Reduced from default
#define UI_LABEL_FONT_SIZE 10      // Reduced from 12px in CSS
#define UI_LIST_MIN_HEIGHT 100     // Reduced from 150

// Playlist Configuration
#define UI_PLAYLIST_ROW_HEIGHT 18  // Reduced from default

// Equalizer Configuration
#define UI_EQ_SLIDER_HEIGHT 60     // Reduced from 80
#define UI_EQ_LABEL_SIZE 8         // Font size for EQ labels

// Volume and Seek Bar
#define UI_SLIDER_HEIGHT 4         // Height of seek/volume sliders (from CSS)

// Playlist Scrolling
#define UI_PLAYLIST_MIN_CONTENT_HEIGHT 100  // Reduced from 150

// Timing and Performance
#define UI_UPDATE_INTERVAL_MS 100   // Update tick interval in milliseconds

// Window Properties
#define UI_WINDOW_RESIZABLE FALSE   // Whether window can be resized by user

// Visualizer
#define VISUALIZER_BAR_WIDTH 3      // Width of individual bars in visualizer
#define VISUALIZER_BAR_GAP 1        // Gap between bars in visualizer

#endif // CONFIG_H
