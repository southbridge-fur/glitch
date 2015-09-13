#if !defined GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <iostream>

using namespace std;

class GUIManager
{
private:
    
public:
    GUIManager();
    virtual ~GUIManager();

    static void activate (GtkApplication* app, gpointer user_data);
    
    static GtkWidget* window;
};


#endif
