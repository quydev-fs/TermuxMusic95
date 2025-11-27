#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>
#include <string>

class Utils {
public:
    // Calculates path relative to the executable
    static std::string getResourcePath(const std::string& assetName);
    
    // Loads the CSS file into the global screen provider
    static void loadGlobalCSS();
    
    // Sets the window icon
    static void setWindowIcon(GtkWidget* window);
    
    // Helper to get an image widget for the About dialog
    static GtkWidget* createLogoImage(int size);
};

#endif
