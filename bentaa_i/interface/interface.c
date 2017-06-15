#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <time.h>
#include <math.h>
#include <gtk/gtk.h>
#include <string.h>
struct items {
    char * path;
    GtkWidget * main_window;
    GtkWidget * load;
    GtkWidget * binarisation;
    GtkWidget * segmentation;
    GtkWidget * reconnaissance;
    GtkWidget * edit;
    GtkWidget * save;
    GtkWidget * quit;
    GtkWidget * image_displayed;
};

struct items * init_items(GtkBuilder * builder)
{
    struct items * item = malloc(sizeof(struct items));
    char * path = malloc(sizeof(char));
    GtkWidget * main_window = malloc(sizeof(GtkWidget));
    GtkWidget * load_main_window = malloc(sizeof(GtkWidget));
    GtkWidget * binarisation_main_window = malloc(sizeof(GtkWidget));
    GtkWidget * edit_main_window = malloc(sizeof(GtkWidget));
    GtkWidget * save_main_window = malloc(sizeof(GtkWidget));
    GtkWidget * image_displayed = malloc(sizeof(GtkWidget));
    GtkWidget * segmentation_main_window = malloc(sizeof(GtkWidget));
    GtkWidget * reconnaissance_main_window = malloc(sizeof(GtkWidget));
    GtkWidget * quit_main_window = malloc(sizeof(GtkWidget));
    path = "logo.png";
    main_window = GTK_WIDGET(gtk_builder_get_object(builder,"main_window"));
    load_main_window = GTK_WIDGET(gtk_builder_get_object(builder,"load_main_window"));
    binarisation_main_window = GTK_WIDGET(gtk_builder_get_object(builder,"binarisation_main_window"));
    edit_main_window = GTK_WIDGET(gtk_builder_get_object(builder,"edit_main_window"));
    save_main_window = GTK_WIDGET(gtk_builder_get_object(builder,"save_main_window"));
    image_displayed = GTK_WIDGET(gtk_builder_get_object(builder,"image_displayed"));
    segmentation_main_window = GTK_WIDGET(gtk_builder_get_object(builder,"segmentation_main_window"));
    reconnaissance_main_window = GTK_WIDGET(gtk_builder_get_object(builder,"reconnaissance_main_window"));
    quit_main_window = GTK_WIDGET(gtk_builder_get_object(builder,"quit_main_window"));
    item->path = path;
    item->main_window = main_window;
    item->load = load_main_window;
    item->binarisation = binarisation_main_window;
    item->segmentation = segmentation_main_window;
    item->reconnaissance = reconnaissance_main_window;
    item->edit = edit_main_window;
    item->save = save_main_window;
    item->quit = quit_main_window;
    item->image_displayed = image_displayed;
    return item;
}

void file_choose(GtkButton * button, struct items * item)
{
    if (button == NULL)
    {
    }
    GtkWidget * parent_window ;  
    parent_window = item->main_window;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    dialog = gtk_file_chooser_dialog_new ("Charger Image",GTK_WINDOW(parent_window),action,("Annuler"),GTK_RESPONSE_CANCEL,("Ouvrir"),GTK_RESPONSE_ACCEPT,NULL);
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        gchar *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        gtk_image_clear(GTK_IMAGE(item->image_displayed));
        item->path = filename;
        gtk_image_set_from_file(GTK_IMAGE(item->image_displayed),filename);
        gtk_widget_set_sensitive(item->binarisation,TRUE);
        gtk_widget_set_sensitive(item->segmentation,FALSE);
        gtk_widget_set_sensitive(item->reconnaissance,FALSE);
        gtk_widget_set_sensitive(item->edit,FALSE);
        gtk_widget_set_sensitive(item->save,FALSE);
        
    }
    gtk_widget_destroy (dialog);
}

void on_window_main_destroy()
{   
    gtk_main_quit();
}

/*
void load_binarized(GtkWidget * widget, struct items * item)
{
    if (widget != NULL){}
    bin_seg(item->path);
    gtk_image_clear(GTK_IMAGE(item->image_displayed));
    gtk_image_set_from_file(GTK_IMAGE(item->image_displayed),"binarized.bmp");
    gtk_widget_set_sensitive(item->segmentation,TRUE);
}

void load_segmented(GtkWidget * widget, struct items * item)
{
    if (widget !=NULL)
    {}
    gtk_image_clear(GTK_IMAGE(item->image_displayed));
    gtk_image_set_from_file(GTK_IMAGE(item->image_displayed),"segmented.bmp");
    gtk_widget_set_sensitive(item->reconnaissance,TRUE);
}

void file_reconnaissance(GtkWidget * widget, struct items * item)
{
    if (widget !=NULL){}
    GtkWidget * dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(item->main_window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"La reconnaissance a été un succès ! Vous pouvez éditer votre document texte en cliquant sur le bouton Editer ou sauvegarder directement ce fichier dans le répertoire de votre choix en cliquant sur Sauvegarder.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_set_sensitive(item->edit,TRUE);
    gtk_widget_set_sensitive(item->save,TRUE);
}

*/
void file_edit(GtkButton * button)
{
    if (!fork() && button != NULL)
    {
        execlp("gedit","gedit","result.txt", NULL);
    }
}

void file_save(GtkButton * button, struct items * item)
{
    if(button){}
    GtkWidget * dialog;
    GtkFileChooser * chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;
    dialog = gtk_file_chooser_dialog_new ("Sauvegarder",GTK_WINDOW(item->main_window),action,("Annuler"),GTK_RESPONSE_CANCEL,("Sauvegarder"),GTK_RESPONSE_ACCEPT,NULL);
    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
    //if (user_edited_a_new_document)
      //  gtk_file_chooser_set_current_name(chooser,("Untitled document"));
    //else
      //  gtk_file_chooser_set_filename(chooser,existing_filename);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char * filename;
        filename = gtk_file_chooser_get_filename(chooser);
        FILE * source, *destination;
        char i;
        source = fopen("result.txt","r");
        destination = fopen(filename,"w");
        while(1)
        {
            i = fgetc(source);
            if (i  == EOF)
                break;
            else
                putc(i,destination);
        }
        fclose(source);
        fclose(destination);            
        //save file
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[])
{
    if (argc > 2 && argv[1] != NULL)
        return 1;
    GtkBuilder * builder;
    GtkWidget * main_window;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();

    gtk_builder_add_from_file(builder,"graphic_interface.glade", NULL);
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    struct items * item = init_items(builder);

    g_signal_connect(G_OBJECT(item->load),"clicked",G_CALLBACK(file_choose),item);
    g_signal_connect(G_OBJECT(item->main_window),"destroy",G_CALLBACK(on_window_main_destroy),NULL);
    g_signal_connect(G_OBJECT(item->edit),"clicked",G_CALLBACK(file_edit),NULL);
 /*   g_signal_connect(G_OBJECT(item->quit),"clicked",G_CALLBACK(on_window_main_destroy),NULL);
    g_signal_connect(G_OBJECT(item->binarisation),"clicked",G_CALLBACK(load_binarized),item);
    g_signal_connect(G_OBJECT(item->segmentation),"clicked",G_CALLBACK(load_segmented),item);*/
    g_signal_connect(G_OBJECT(item->save),"clicked",G_CALLBACK(file_save),item); 
   /* g_signal_connect(G_OBJECT(item->reconnaissance),"clicked",G_CALLBACK(file_reconnaissance),item);*/
    
    gtk_widget_show(main_window);

    gtk_main();

    return 0;
}


