#include "player.h"
#include <iostream>

Player::Player(AppState* state) : app(state) {
    gst_init(NULL, NULL);
    pipeline = gst_element_factory_make("playbin", "player");
    
    // Watch bus for EOS (End of Stream)
    GstBus* bus = gst_element_get_bus(pipeline);
    gst_bus_add_watch(bus, busCallback, this);
    gst_object_unref(bus);
}

Player::~Player() {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}

void Player::load(const std::string& uri) {
    stop();
    std::string gstUri = "file://" + uri;
    g_object_set(G_OBJECT(pipeline), "uri", gstUri.c_str(), NULL);
}

void Player::play() {
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    app->playing = true;
    app->paused = false;
}

void Player::pause() {
    gst_element_set_state(pipeline, GST_STATE_PAUSED);
    app->playing = false;
    app->paused = true;
}

void Player::stop() {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    app->playing = false;
    app->paused = false;
}

gboolean Player::busCallback(GstBus* bus, GstMessage* msg, gpointer data) {
    Player* player = (Player*)data;
    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
            // Handle End Of Stream (Next track logic would go here)
            // For now, just stop
            player->stop();
            break;
        case GST_MESSAGE_ERROR:
            // Handle Error
            player->stop();
            break;
        default:
            break;
    }
    return TRUE;
}
