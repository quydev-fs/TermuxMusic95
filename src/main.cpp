#include "ui.h"
#include "player.h"
#include "common.h"
#include "playlist.h"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Starting TermuxMusic95..." << std::endl;

    AppState appState;
    
    // Logic moved to playlist.cpp
    loadPlaylist(appState, argc, argv);

    if (appState.playlist.empty()) {
        std::cout << "Usage: ./TermuxMusic95 <file.mp3> OR <folder> OR <list.m3u>" << std::endl;
        return 1;
    }

    UI ui(&appState);
    if (!ui.init()) {
        std::cerr << "Failed to initialize X11 Display." << std::endl;
        return 1;
    }

    Player player(&appState);
    player.start();

    ui.runLoop();

    player.stop();
    return 0;
}
