# TermAMP

<p align="center">
  <img src="assets/icons/logo.jpg" alt="logo"/>
</p>

*An GTK-based MP3 player for Termux, inspired by WinAMP 2.x and written entirely in C++.*

***

## 🚀 Overview

**TermAMP** brings the nostalgic look and functionality of WinAMP 2.x to Android’s Termux environment. With a familiar GUI rendered via X11, robust MP3 playback thanks to `mpg123`, PulseAudio integration, and real-time spectrum visualizations via FFT, this project demonstrates what’s possible with modern terminal environments on Android.

***

## ✨ Features

- Classic WinAMP-inspired graphical user interface (GTK)
**Universal audio format support**: MP3, FLAC, OGG, AAC, WAV, Opus, M4A, WMA, ALAC, APE, and more!
- 📊 Real-time waveform visualizer
- PulseAudio streaming
- Real-time spectrum analyzer (FFT-based)
- Keyboard and mouse controls
- Playlist and folder support

***
## 🚀 Quick Start

```sh
# Clone and build
git clone https://github.com/quydev-fs/TermAMP.git
cd TermAMP
make

# Run
./build/bin/TermAMP
```

For detailed build instructions, see [BUILDING.md](docs/BUILDING.md)

***

## 📂 Project Structure
```
TermAMP/
├── src/                 # C++ source files
│   ├── main.cpp        # Application entry point
│   ├── player.cpp      # Audio playback engine
│   ├── playlist.cpp    # Playlist management
│   ├── ui.cpp          # Terminal user interface
│   └── visualizer.cpp  # Audio visualization
├── include/            # Header files
│   ├── common.h        # Common definitions & utilities
│   ├── player.h
│   ├── playlist.h
│   ├── ui.h
│   └── visualizer.h
├── build/              # Build artifacts (generated)
│   ├── obj/           # Object files
│   └── bin/           # Executable output
├── assets/             # Application resources
│   ├── icons/
│   │   └── logo.png   # Application and readme logo
│   ├── style.css
│   └── screenshots/   # Application screenshots
│       ├── termamp-full-idle.png
│       ├── termamp-full-playing.png
│       ├── termamp-mini-idle.png
│       └── termamp-mini-playing.png
├── Makefile           # Build configuration
└── README.md          # Project documentation
```
***

## 📸 Screenshots

for screenshots, go to [docs/SCREENSHOTS.md](docs/SCREENSHOTS.md)

***

## ▶️ Usage

The player takes a file, directory, or `.m3u` playlist as a command-line argument:

```sh
# Start TermAMP without loading anything
./build/bin/TermAMP
# Play a single file
./build/bin/TermAMP /sdcard/Music/track.mp3

# Play all MP3s in a directory
./build/bin/TermAMP /sdcard/Music/AlbumFolder

# Play using an M3U playlist
./build/bin/TermAMP /sdcard/Playlists/playlist.m3u
```

***

## ⌨️ Controls

**Mouse:**  
- Click buttons, seek bar, and drag the volume slider

**Keyboard shortcuts:**

| Key        | Action             |
|------------|--------------------|
| Z          | Previous Track     |
| X          | Play/Resume        |
| C          | Pause              |
| V          | Stop/Reset         |
| B          | Next Track         |
| Up Arrow   | Volume Up          |
| Down Arrow | Volume Down        |

***

## 📄 License

This project is licensed under the [MIT License](LICENSE).

***

## 🙋 FAQ & Contribution

- **Bugs and feature requests:** Please open issues and PRs on GitHub.
- **Contact:** See repository issues or discussions for support

*"it may better when it comes to retro"*
