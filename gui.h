#if !defined GUI_H
#define GUI_H

#include <iostream>

using namespace std;

typedef struct _GtkApplication GtkApplication;
typedef struct _GtkWidget GtkWidget;
typedef void* gpointer;

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
