#include "utils.h"
#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <cstdlib>

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
