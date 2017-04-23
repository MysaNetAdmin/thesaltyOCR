#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <gtk/gtk.h>

* Well, by golly, this program uses widgets: */
typedef struct {
	GtkWidget *window1;
	GtkWidget *entry1;
	GtkWidget *entry2;
	GtkWidget *label1;
} AppWidgets;

/* Make the copy, cut, paste, delete menu items work */
#if 1
#define UNUSED gpointer _x_ G_GNUC_UNUSED
#define FOCUSED GTK_EDITABLE( \
	(GTK_WIDGET_HAS_FOCUS(app->entry2))?app->entry2:app->entry1)
#define EDIT(copy,clipboard) \
void edit_##copy(UNUSED, AppWidgets *app){ \
	gtk_editable_##copy##_##clipboard(FOCUSED); }
EDIT(copy,clipboard)
EDIT(cut,clipboard)
EDIT(paste,clipboard)
EDIT(delete,selection)
#undef EDIT
#endif

void file_save_as(UNUSED){
}

void file_save(GtkWidget *widget){
}

void file_open(GtkWidget *widget){
}

void file_new(GtkWidget *widget){
}

/* Show the About box */
void help_about(GtkWidget *widget){
	gtk_dialog_run(GTK_DIALOG(widget));
	gtk_widget_hide(widget);
}

/* Somebody clicked on our web page link */
void link_click(GtkWidget *widget){
	const gchar *link=gtk_link_button_get_uri(GTK_LINK_BUTTON(widget));
	gchar *lloc=g_strconcat("htmlview ",link,NULL);
	system(lloc);
	g_slice_free(gchar,lloc);
	g_printf("clicked %s\n",gtk_widget_get_name(widget));
}

/* Serve up 32 hex digits. Must be freed */
gchar *make_hash(const gchar *input){
	int i;
	unsigned char bytes[16];
	gchar hexdigits[2];
	gchar *hexstring=g_malloc(32);hexstring[0]='\0';
	//MD5 returns 16 bytes.
	MD5((unsigned char*)input,g_utf8_strlen(input,99),bytes);
	for(i=0;i<16;i++){ //Let's convert them to 32 hex digits.
		g_sprintf(hexdigits,"%02x",bytes[i]);
		g_strlcat(hexstring,hexdigits,32);
	}
	return hexstring;
}

/* Somebody pressed the Check Hash button */
void check_hash(UNUSED, AppWidgets *app) {
	gchar *hash=make_hash(gtk_entry_get_text(GTK_ENTRY(app->entry1)));
	if(!g_strcmp0(hash,gtk_entry_get_text(GTK_ENTRY(app->entry2))))
		gtk_label_set_text(GTK_LABEL(app->label1),"valid");
	else
		gtk_label_set_text(GTK_LABEL(app->label1),"fail");
	g_free(hash);
}

/* Somebody pressed the Generate Hash button */
void generate_hash(UNUSED, AppWidgets *app) {
	gchar *hash=make_hash(gtk_entry_get_text(GTK_ENTRY(app->entry1)));
	gtk_entry_set_text(GTK_ENTRY(app->entry2),hash);
	g_free(hash);
}

/* Somebody pressed the ESC key */
gboolean key_pressed(UNUSED, GdkEventKey *event){
    if (event->keyval==GDK_Escape) gtk_main_quit();
	return FALSE;//not done yet. get more keys!
}

/* Somebody ran our program */
int main(int argc, char **argv) {
	GtkBuilder *builder;
	AppWidgets *app = g_slice_new(AppWidgets);
	GError *err = NULL;
	
	gtk_init(&argc, &argv);
	builder=gtk_builder_new();
	gtk_builder_add_from_file(builder, "hasher.xml", &err);
	
	if (err) {
		g_error(err->message);
		g_error_free(err);
		g_slice_free(AppWidgets, app);
		return 1;
	}
	
	//Get handles to our app widgets.
	#define appGET(xx) \
	app->xx=GTK_WIDGET(gtk_builder_get_object(builder,#xx))
	appGET(entry1);
	appGET(entry2);
	appGET(label1);
	appGET(window1);
	gtk_builder_connect_signals(builder, app);
	g_object_unref(G_OBJECT(builder));
	
	gtk_main();
	g_slice_free(AppWidgets, app);
	return 0;
}

