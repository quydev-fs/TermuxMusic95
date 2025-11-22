#include "playlist.h"
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <numeric> 
#include <random>  
#include <chrono>  
#include <sstream>

void loadPlaylist(AppState& app, int argc, char** argv) {
    if (argc < 2) return;

    app.playlist.clear();
    app.play_order.clear();

    struct stat s;
    if (stat(argv[1], &s) == 0) {
        if (s.st_mode & S_IFDIR) {
            DIR *dir; struct dirent *ent;
            if ((dir = opendir(argv[1])) != NULL) {
                while ((ent = readdir(dir)) != NULL) {
                    std::string fname = ent->d_name;
                    if(fname.length() > 4 && fname.substr(fname.length()-4) == ".mp3") {
                         app.playlist.push_back(std::string(argv[1]) + "/" + fname);
                    }
                }
                closedir(dir);
            }
        } else {
            std::string arg = argv[1];
            if (arg.substr(arg.length()-4) == ".m3u") {
                FILE* f = fopen(arg.c_str(), "r");
                char line[256];
                while(fgets(line, sizeof(line), f)) {
                    if(line[0] != '#') {
                        std::string l = line;
                        l.erase(std::remove(l.begin(), l.end(), '\n'), l.end());
                        if(!l.empty()) app.playlist.push_back(l);
                    }
                }
                fclose(f);
            } else {
                for(int i=1; i<argc; i++) app.playlist.push_back(argv[i]);
            }
        }
    }
    std::sort(app.playlist.begin(), app.playlist.end());
    
    app.play_order.resize(app.playlist.size());
    std::iota(app.play_order.begin(), app.play_order.end(), 0);
}

void toggleShuffle(AppState& app) {
    if (app.playlist.empty()) return;

    if (app.play_order.size() != app.playlist.size()) {
        app.play_order.resize(app.playlist.size());
        std::iota(app.play_order.begin(), app.play_order.end(), 0);
    }

    size_t current_real_index = 0;
    if (app.track_idx < app.play_order.size()) {
        current_real_index = app.play_order[app.track_idx];
    }

    if (app.shuffle) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 g(seed);
        std::shuffle(app.play_order.begin(), app.play_order.end(), g);
    } else {
        std::iota(app.play_order.begin(), app.play_order.end(), 0);
    }

    for(size_t i=0; i<app.play_order.size(); i++) {
        if(app.play_order[i] == current_real_index) {
            app.track_idx = i;
            break;
        }
    }
    
    std::cout << "Shuffle Toggled." << std::endl;
}

bool savePlaylist(const AppState& app, std::string filename) {
    if (app.playlist.empty()) return false;

    if (filename.empty()) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << "playlist_" << std::put_time(&tm, "%Y-%m-%d_%H%M%S") << ".m3u";
        filename = oss.str();
    }

    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "#EXTM3U" << std::endl;
    for (const auto& track : app.playlist) {
        file << track << std::endl;
    }
    file.close();
    return true;
}
