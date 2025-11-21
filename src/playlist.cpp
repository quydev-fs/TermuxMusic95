#include "playlist.h"
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <cstdio>

void loadPlaylist(AppState& app, int argc, char** argv) {
    if (argc < 2) return;

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
}
