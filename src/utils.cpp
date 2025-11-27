#include "utils.h"
#include "common.h"
#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <cstdlib>
#include <gst/gst.h>
#include <thread>

// Static member initialization
Utils::ConversionProgressCallback Utils::progressCallback = nullptr;
void* Utils::callbackUserData = nullptr;

std::string Utils::getResourcePath(const std::string& assetName) {
    // 1. Check standard Termux prefix
    std::string configDir;
    const char* env_prefix = std::getenv("PREFIX");

    if (env_prefix) {
        configDir = std::string(env_prefix) + "/etc/TermAMP/";
    } else {
        // 2. Fallback: Relative to binary (for dev/testing)
        char result[PATH_MAX];
        for(int i=0; i<PATH_MAX; ++i) result[i] = 0;
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            std::string exePath(result, count);
            std::string binDir = exePath.substr(0, exePath.find_last_of("/"));
            return binDir + "/../../" + assetName;
        }
        return assetName;
    }

    // 3. Strip "assets/" prefix if using system install (/etc/TermAMP)
    std::string cleanName = assetName;
    std::string removePrefix = "assets/";
    if (cleanName.rfind(removePrefix, 0) == 0) {
        cleanName = cleanName.substr(removePrefix.length());
    }
    return configDir + cleanName;
}

void Utils::loadGlobalCSS() {
    GtkCssProvider *provider = gtk_css_provider_new();
    std::string cssPath = getResourcePath("assets/style.css");

    GError *error = NULL;
    gtk_css_provider_load_from_path(provider, cssPath.c_str(), &error);

    if (error) {
        std::cerr << "[Utils] CSS Error: " << error->message << std::endl;
        g_error_free(error);
    } else {
        gtk_style_context_add_provider_for_screen(
            gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
    }
    g_object_unref(provider);
}

void Utils::setWindowIcon(GtkWidget* window) {
    std::string logoPath = getResourcePath("assets/icons/logo.jpg");
    GError *err = NULL;
    if(!gtk_window_set_icon_from_file(GTK_WINDOW(window), logoPath.c_str(), &err)) {
        if(err) g_error_free(err);
    }
}

GtkWidget* Utils::createLogoImage(int size) {
    std::string path = getResourcePath("assets/icons/logo.jpg");
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale(path.c_str(), size, size, TRUE, NULL);
    if (!pixbuf) return gtk_image_new(); // Return empty if fail

    GtkWidget* img = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);
    return img;
}

// Audio conversion implementation
std::vector<std::string> Utils::getSupportedOutputFormats() {
    return {"mp3", "wav", "flac", "ogg", "aac", "m4a", "opus"};
}

bool Utils::isFormatSupported(const std::string& format) {
    auto formats = getSupportedOutputFormats();
    for (const auto& fmt : formats) {
        if (fmt == format) return true;
    }
    return false;
}

void Utils::setProgressCallback(ConversionProgressCallback callback, void* userData) {
    progressCallback = callback;
    callbackUserData = userData;
}

bool Utils::convertAudioFormat(const std::string& inputPath, const std::string& outputPath, const std::string& format) {
    if (!isFormatSupported(format)) {
        std::cerr << "Unsupported format: " << format << std::endl;
        return false;
    }

    // Initialize GStreamer if not already done
    static bool gstInitialized = false;
    if (!gstInitialized) {
        gst_init(NULL, NULL);
        gstInitialized = true;
    }

    // Create a simple pipeline to convert the audio
    std::string pipelineStr;
    if (format == "mp3") {
        pipelineStr = "filesrc location=\"" + inputPath + "\" ! decodebin ! audioconvert ! audioresample ! lamemp3enc ! mp3parse ! filesink location=\"" + outputPath + "\"";
    } else if (format == "wav") {
        pipelineStr = "filesrc location=\"" + inputPath + "\" ! decodebin ! audioconvert ! audioresample ! wavenc ! filesink location=\"" + outputPath + "\"";
    } else if (format == "flac") {
        pipelineStr = "filesrc location=\"" + inputPath + "\" ! decodebin ! audioconvert ! audioresample ! flacenc ! filesink location=\"" + outputPath + "\"";
    } else if (format == "ogg") {
        pipelineStr = "filesrc location=\"" + inputPath + "\" ! decodebin ! audioconvert ! audioresample ! vorbisenc ! oggmux ! filesink location=\"" + outputPath + "\"";
    } else if (format == "aac") {
        pipelineStr = "filesrc location=\"" + inputPath + "\" ! decodebin ! audioconvert ! audioresample ! faac ! mp4mux ! filesink location=\"" + outputPath + "\"";
    } else if (format == "m4a") {
        pipelineStr = "filesrc location=\"" + inputPath + "\" ! decodebin ! audioconvert ! audioresample ! faac ! mp4mux ! filesink location=\"" + outputPath + "\"";
    } else if (format == "opus") {
        pipelineStr = "filesrc location=\"" + inputPath + "\" ! decodebin ! audioconvert ! audioresample ! opusenc ! oggmux ! filesink location=\"" + outputPath + "\"";
    } else {
        std::cerr << "Format not implemented: " << format << std::endl;
        return false;
    }

    GError *error = NULL;
    GstElement *pipeline = gst_parse_launch(pipelineStr.c_str(), &error);

    if (error) {
        std::cerr << "Failed to create pipeline: " << error->message << std::endl;
        g_error_free(error);
        return false;
    }

    // Set pipeline to playing state
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Wait for the end of stream or error
    GstBus *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
        (GstMessageType)(GST_MESSAGE_EOS | GST_MESSAGE_ERROR));

    // Stop the pipeline
    gst_element_set_state(pipeline, GST_STATE_NULL);

    bool result = false;
    if (msg != NULL) {
        if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_EOS) {
            result = true;
        } else if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
            GError *err = NULL;
            gchar *dbg_info = NULL;
            gst_message_parse_error(msg, &err, &dbg_info);
            std::cerr << "Error converting audio: " << err->message << std::endl;
            g_error_free(err);
            g_free(dbg_info);
        }
        gst_message_unref(msg);
    }

    gst_object_unref(bus);
    gst_object_unref(pipeline);

    return result;
}
