#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <gtk/gtk.h>

GtkBuilder    *builder;
GtkWidget     *main_window;
GtkWidget     *file_dialog;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "interface.glade", NULL);
 
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "interface"));
    file_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "file_dialog"));
    gtk_builder_connect_signals(builder, NULL);
 
    g_object_unref(builder);
 
    gtk_widget_show(main_window); 
    gtk_main();
 
    return 0;
}

void file_chooser_dialog()
{
    gtk_widget_show(file_dialog);
}
 
// called when window is closed
void on_main_window_destroy()
{
    gtk_main_quit();
}
