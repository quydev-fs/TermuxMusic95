#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include <gst/gst.h>

class Player {
public:
    Player(AppState* state);
    ~Player();

    void load(const std::string& uri);
    void play();
    void pause();
    void stop();
    void setVolume(double vol);
    bool isPlaying();
    
    // Auto-advance callback
    static gboolean busCallback(GstBus* bus, GstMessage* msg, gpointer data);

private:
    AppState* app;
    GstElement* pipeline;
};

#endif
