#include <gnome.h>

#define X_APPNAME "exclude"
#define X_WINNAME "Exclude"
#define X_VERSION "0.0.1"

void
destroy (GtkWidget *widget, gpointer *data)
{
	gtk_exit (0);
}

int
main (int argc, char *argv[])
{
	GtkWidget *app		= NULL;
	GtkWidget *scroll	= NULL;
	GtkWidget *table	= NULL;
	GtkWidget *clist	= NULL;
	GtkWidget *add		= NULL;
	GtkWidget *remove	= NULL;
	GtkWidget *close	= NULL;
	GtkWidget *bbox		= NULL;
	GtkWidget *text		= NULL;

	gnome_init (X_APPNAME, X_VERSION, argc, argv);

	app	= gnome_app_new (X_APPNAME, X_WINNAME);
	table	= gtk_table_new (3, 1, FALSE);
	scroll	= gtk_scrolled_window_new (NULL, NULL);
	clist	= gtk_clist_new (2);
	text	= gtk_entry_new();
	bbox	= gtk_hbutton_box_new();
	add   	= gtk_button_new_with_label ("Add");
	remove	= gtk_button_new_with_label ("Remove");
	close 	= gtk_button_new_with_label ("Close");

	gtk_button_box_set_layout     (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing    (GTK_BUTTON_BOX (bbox), 10);

	gnome_app_set_contents	(GNOME_APP	(app),    table);

	//gtk_container_add	(GTK_CONTAINER	(table),  scroll);
	//gtk_container_add	(GTK_CONTAINER	(table),  text);
	//gtk_container_add	(GTK_CONTAINER	(table),  bbox);

	gtk_table_attach (GTK_TABLE (table), scroll, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND | GTK_SHRINK, GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
	gtk_table_attach (GTK_TABLE (table), text,   0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
	gtk_table_attach (GTK_TABLE (table), bbox,   0, 1, 2, 3, GTK_FILL, GTK_FILL, 0, 0);

	gtk_container_add	(GTK_CONTAINER	(scroll), clist);
	gtk_container_add	(GTK_CONTAINER	(bbox),   add);
	gtk_container_add	(GTK_CONTAINER	(bbox),   remove);
	gtk_container_add	(GTK_CONTAINER	(bbox),   close);

	gtk_signal_connect (GTK_OBJECT (close), "clicked", (GtkSignalFunc) destroy, NULL);
	gtk_signal_connect (GTK_OBJECT (app),   "destroy", (GtkSignalFunc) destroy, NULL);
	
	gtk_widget_show_all (app);
	gtk_main();
	
	return 0;
}
