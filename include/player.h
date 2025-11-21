#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include <pthread.h>

class Player {
public:
    Player(AppState* state);
    ~Player();
    void start();
    void stop();

private:
    static void* threadEntry(void* arg);
    void audioLoop();
    AppState* app;
    pthread_t threadId;
};

#endif
