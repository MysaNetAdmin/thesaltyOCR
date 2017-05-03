#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <gtk/gtk.h>

GtkBuilder    *builder;
GtkWidget     *main_window;
GtkWidget     *image;
GtkWidget     *open, *bin, *xor, *save, *cancel;
GtkWidget     *file_dialog;

static void chooser_dialog()
{
  gtk_widget_show(file_dialog);
}

static void close_dialog()
{
  gtk_widget_hide(file_dialog);
}

static void save_clicked()
{
  const gchar *filename;
  GtkFileChooser *chooser = gtk_file_chooser(file_dialog);
  filename = gtk_file_chooser_get_filename(chooser);
  gtk_image_set_from_file(image, filename);
  close_dialog();
}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "interface.glade", NULL);

  main_window = GTK_WIDGET(gtk_builder_get_object(builder, "interface"));
  file_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "file_dialog"));
  image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
  open = GTK_WIDGET(gtk_builder_get_object(builder, "open"));
  bin = GTK_WIDGET(gtk_builder_get_object(builder, "bin"));
  xor = GTK_WIDGET(gtk_builder_get_object(builder, "xor"));
  save = GTK_WIDGET(gtk_builder_get_object(builder, "save"));
  cancel = GTK_WIDGET(gtk_builder_get_object(builder, "cancel"));

  gtk_builder_connect_signals(builder, NULL);

  g_signal_connect_swapped(open, "clicked", G_CALLBACK(chooser_dialog), NULL);
  g_signal_connect_swapped(save, "clicked", G_CALLBACK(save_clicked), NULL);
  g_signal_connect_swapped(cancel, "clicked", G_CALLBACK(close_dialog), NULL);
 
  g_object_unref(builder);
 
  gtk_widget_show(main_window);
  gtk_main();

  return 0;
}
