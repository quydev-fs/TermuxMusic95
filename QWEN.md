# TermAMP Project Analysis

## Overview
TermAMP is a GTK-based MP3 player for Termux and Linux that recreates the UI of WinAMP version 2.x. It's written entirely in C++ and uses GStreamer for audio playback. The project provides a nostalgic WinAMP 2.x-like interface with modern audio format support and real-time visualizations.

## Project Structure
```
TermAMP/
├── src/                 # C++ source files
│   ├── main.cpp        # Application entry point
│   ├── player.cpp      # Audio playback engine (GStreamer)
│   ├── playlist.cpp    # Playlist management
│   ├── ui.cpp          # Graphical user interface (GTK)
│   ├── visualizer.cpp  # Audio visualization
│   └── utils.cpp       # Utility functions
├── include/            # Header files
│   ├── common.h        # Shared definitions and state
│   ├── player.h
│   ├── playlist.h
│   ├── ui.h
│   ├── visualizer.h
│   └── utils.h
├── build/              # Build artifacts (generated)
├── assets/             # Application resources
│   ├── icons/
│   ├── style.css       # Custom GTK theming
│   └── screenshots/
├── Makefile           # Build configuration
├── README.md
├── docs/              # Documentation
│   ├── BUILDING.md
│   └── SCREENSHOTS.md
```

## Libraries and Dependencies

### Primary Libraries:
1. **GTK 3** (`gtk+-3.0`) - Provides the graphical user interface
2. **GStreamer 1.0** (`gstreamer-1.0`) - Handles audio playback
3. **Additional GStreamer plugins**:
   - `gst-plugins-base`
   - `gst-plugins-good` 
   - `gst-plugins-bad`
   - `gst-plugins-ugly`
   - `gst-libav`

### System Dependencies:
- `termux-x11-nightly` (for Termux environment)
- `pkg-config` (for build configuration)
- `clang` (C++ compiler)
- `make` (build system)

## Function Syntax and Code Patterns

### GTK Usage Patterns:
1. **Signal Connections**:
   ```cpp
   g_signal_connect(button, "clicked", G_CALLBACK(callback), this);
   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
   ```

2. **Widget Creation**:
   ```cpp
   GtkWidget* button = gtk_button_new_with_label("Play");
   GtkWidget* label = gtk_label_new("Text");
   GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
   ```

3. **Callback Functions**:
   ```cpp
   static gboolean callback(GtkWidget* widget, GdkEvent* event, gpointer data) {
       // Cast data back to class instance
       UI* ui = (UI*)data;
       return TRUE;
   }
   ```

4. **Memory Management**:
   - Use of `g_object_ref()` and `g_object_unref()` for reference counting
   - Proper GTK widget lifecycle management
   - Proper GStreamer object unref'ing with `gst_object_unref()`

### GStreamer Implementation:
1. **Pipeline Creation**:
   ```cpp
   pipeline = gst_element_factory_make("playbin", "player");
   GstBus* bus = gst_element_get_bus(pipeline);
   gst_bus_add_watch(bus, busCallback, this);
   ```

2. **State Management**:
   - `GST_STATE_PLAYING`, `GST_STATE_PAUSED`, `GST_STATE_NULL`
   - Position/duration queries using `gst_element_query_position()`

3. **Message Handling**:
   - EOS (End of Stream) handling with `GST_MESSAGE_EOS`
   - Error handling with `GST_MESSAGE_ERROR`
   - Tag parsing with `GST_MESSAGE_TAG`

### Class Architecture:
- **AppState**: Global application state management
- **Player**: Audio playback using GStreamer
- **PlaylistManager**: Playlist management and track navigation
- **Visualizer**: Real-time audio visualization
- **UI**: Main application interface using GTK
- **Utils**: Resource management and utility functions

## GTK Library Specific Notes

### Widget Hierarchy:
- Window → Main Box → Menu Bar, Visualizer Container, Info Label, Seek Bar, Volume Control, Controls Box, Playlist Scrolled Window

### Custom Styling:
- Uses CSS theming via `style.css`
- Custom styling classes like `.tm-window`, `.active-mode`
- WinAMP-like color scheme with dark background and green text

### Event Handling:
- Mouse events (button clicks, slider adjustments)
- Keyboard shortcuts (space for play/pause, arrows for volume, etc.)
- Custom drawing for visualizer using Cairo

## Key Features

### Playback Features:
- Support for multiple audio formats (MP3, FLAC, OGG, AAC, WAV, etc.)
- Playlist management with drag-and-drop and M3U support
- Real-time spectrum analyzer/visualizer
- Shuffle and repeat modes
- Volume control and seeking

### UI Features:
- WinAMP 2.x-inspired interface
- Mini/Full mode toggle
- Custom CSS styling for authentic look
- Keyboard shortcuts matching WinAMP conventions
- Playlist browser with track selection

### Technical Features:
- Cross-platform compatibility (Termux/Android and Linux)
- X11 integration for Termux GUI
- GStreamer-based audio engine for format support
- Reference-counted resource management
- Thread-safe UI updates

## Architecture Summary

The project implements a multimedia player with:
1. **Player** class managing GStreamer pipeline
2. **UI** class managing the GTK interface
3. **PlaylistManager** handling tracks and navigation
4. **Visualizer** providing real-time audio visualization
5. **Utils** for resource management
6. **Common** header for shared state

The application follows standard GTK patterns for signal handling, widget creation, and event-driven programming, while integrating GStreamer for robust audio playback capabilities.

## Build Process

The Makefile uses pkg-config to locate GTK and GStreamer libraries, compiles all source files, and links against the necessary libraries. The build system creates object files in `build/obj/` and produces the executable in `build/bin/`.

@COMMIT_RULES.md
@TODO.md