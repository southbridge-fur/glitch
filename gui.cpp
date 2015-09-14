#include "gui.h"

#include <gtk/gtk.h>

GUIManager::GUIManager()
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.dolphinhats.glitch", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);        
}

GUIManager::~GUIManager()
{
    //delete everything
}

void GUIManager::activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *image;
    GtkWidget *picker, *table, *label;
    GtkWidget *imageBox;

    GtkWidget *bbox;
    GtkWidget *button;
    
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Test Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);

    table = gtk_grid_new ();
    gtk_grid_set_row_spacing (GTK_GRID (table), 10);
    gtk_grid_set_row_homogeneous (GTK_GRID (table), true);
    gtk_grid_set_baseline_row (GTK_GRID (table),0);
    
    gtk_grid_set_column_spacing (GTK_GRID (table), 10);
    gtk_grid_set_column_homogeneous (GTK_GRID (table), true);
    
    gtk_container_add (GTK_CONTAINER (window), table);    

    label = gtk_label_new ("File:");
    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand (label, TRUE);
    picker = gtk_file_chooser_button_new ("Pick a File",
                                          GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_grid_attach (GTK_GRID (table), label, 0, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (table), picker, 1, 0, 1, 1);

    imageBox = gtk_frame_new (NULL);
    gtk_grid_attach (GTK_GRID (table), imageBox, 2, 0, 3, 3);

    bbox = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach (GTK_GRID (table), bbox, 0, 2, 3, 1);
/*
    button = gtk_button_new_with_label (("OK"));
    gtk_container_add (GTK_CONTAINER (bbox), button);
    
    g_signal_connect (G_OBJECT (button), "start",
		      G_CALLBACK (), NULL);
*/  
    //give file preview

    //go

    
    gtk_widget_show_all(window);
}
