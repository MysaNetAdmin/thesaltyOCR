#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <gtk/gtk.h>
//#include "../segmentation/pixel_operations.c"
//#include "../segmentation/main.c"

GtkBuilder    *builder;
GtkWidget     *main_window;
GtkWidget     *image;
GtkWidget     *open, *bin, *xor, *pWindow;
const gchar *sText;
char img_name[500] = "";

static void chooser_dialog()
{
  gtk_widget_show(pWindow);
  gtk_window_set_modal(GTK_WINDOW(pWindow), TRUE);
}

void on_activate_entry(GtkWidget *pEntry, gpointer data)
{ 
    sText = gtk_entry_get_text(GTK_ENTRY(pEntry));

    gtk_label_set_text(GTK_LABEL((GtkWidget*)data), sText);
    gtk_image_set_from_file(GTK_IMAGE(image), sText);
}

void on_copier_button(GtkWidget *pButton, gpointer data)
{
    GtkWidget *pTempEntry;
    GtkWidget *pTempLabel;
    GList *pList;
    
    gtk_button_set_label(GTK_BUTTON(pButton), "Ouvrir l'image");
 
    pList = gtk_container_get_children(GTK_CONTAINER((GtkWidget*)data));
 
    pTempEntry = GTK_WIDGET(pList->data);
 
    pList = g_list_next(pList);
 
    pList = g_list_next(pList);

    pTempLabel = GTK_WIDGET(pList->data);
 
    sText = gtk_entry_get_text(GTK_ENTRY(pTempEntry));
 
    gtk_label_set_text(GTK_LABEL(pTempLabel), sText);
    gtk_image_set_from_file(GTK_IMAGE(image), sText);
    img_name[500] = (char)(sText);
 
    g_list_free(pList);
    gtk_widget_hide(pWindow);
}

void black_white()
{
  //SDL_Surface *img = load_image((char*)(sText));
  //display_black_n_white(img);
}

int main(int argc, char *argv[])
{ 
  GtkWidget *pVBox;
  GtkWidget *pEntry;
  GtkWidget *pButton;
  GtkWidget *pLabel;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "interface.glade", NULL);

  main_window = GTK_WIDGET(gtk_builder_get_object(builder, "interface"));
  image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
  open = GTK_WIDGET(gtk_builder_get_object(builder, "open"));
  bin = GTK_WIDGET(gtk_builder_get_object(builder, "bin"));
  xor = GTK_WIDGET(gtk_builder_get_object(builder, "xor"));
  pWindow = GTK_WIDGET(gtk_builder_get_object(builder, "pWindow"));
  pVBox = GTK_WIDGET(gtk_builder_get_object(builder, "pVBox"));
  pEntry = GTK_WIDGET(gtk_builder_get_object(builder, "pEntry")); 
  pButton= GTK_WIDGET(gtk_builder_get_object(builder, "pButton"));
  pLabel = GTK_WIDGET(gtk_builder_get_object(builder, "pLabel"));

  gtk_builder_connect_signals(builder, NULL);

  g_signal_connect_swapped(open, "clicked", G_CALLBACK(chooser_dialog), NULL);

  g_signal_connect_swapped(bin, "clicked", G_CALLBACK(black_white), NULL);

  g_signal_connect(G_OBJECT(pEntry), "activate", G_CALLBACK(on_activate_entry), (GtkWidget*) pLabel);
 
  g_signal_connect(G_OBJECT(pButton), "clicked", G_CALLBACK(on_copier_button), (GtkWidget*) pVBox);
 
  g_object_unref(builder);
 
  gtk_widget_show(main_window);
  gtk_main();

  return 0;
}
