#include <gnome.h>

#define X_APPNAME "exclude"
#define X_WINNAME "Exclude"
#define X_VERSION "0.0.1"

GtkWidget *app		= NULL;
GtkWidget *scroll	= NULL;
GtkWidget *table	= NULL;
GtkWidget *clist	= NULL;
GtkWidget *bbox		= NULL;
GtkWidget *text		= NULL;

void
destroy (GtkWidget *widget, gpointer *data)
{
	gtk_exit (0);
}

void
add_text (GtkWidget *widget, gpointer *data)
{
	char *cols[2] = { "", "" };
	GString *str = g_string_new (gtk_entry_get_text (GTK_ENTRY (text)));

	gtk_entry_set_text (GTK_ENTRY (text), "");
	cols[0] = str->str;
	cols[1] = str->str;

	gtk_clist_append (GTK_CLIST (clist), cols);
}

void
remove_text (GtkWidget *widget, gpointer *data)
{
	g_print ("remove\n");
}

int
main (int argc, char *argv[])
{
	GtkWidget *add		= NULL;
	GtkWidget *remove	= NULL;
	GtkWidget *close	= NULL;
	GtkAccelGroup *accel    = NULL;

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

	gtk_label_set_pattern (GTK_LABEL (GTK_BIN (add)   ->child), "_");
	gtk_label_set_pattern (GTK_LABEL (GTK_BIN (remove)->child), "_");
	gtk_label_set_pattern (GTK_LABEL (GTK_BIN (close) ->child), "_");

	accel = gtk_accel_group_new ();

	gtk_accel_group_attach (accel, GTK_OBJECT (app));

	gtk_widget_add_accelerator (add,    "clicked", accel, GDK_a, GDK_MOD1_MASK, 0);
	gtk_widget_add_accelerator (remove, "clicked", accel, GDK_r, GDK_MOD1_MASK, 0);
	gtk_widget_add_accelerator (close,  "clicked", accel, GDK_c, GDK_MOD1_MASK, 0);

	gtk_button_box_set_layout     (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing    (GTK_BUTTON_BOX (bbox), 10);

	gnome_app_set_contents (GNOME_APP (app), table);

	gtk_table_attach (GTK_TABLE (table), scroll, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND | GTK_SHRINK, GTK_FILL | GTK_EXPAND | GTK_SHRINK, 10, 10);
	gtk_table_attach (GTK_TABLE (table), text,   0, 1, 1, 2, GTK_FILL, GTK_FILL, 10, 10);
	gtk_table_attach (GTK_TABLE (table), bbox,   0, 1, 2, 3, GTK_FILL, GTK_FILL, 5, 5);

	gtk_container_add (GTK_CONTAINER (scroll), clist);
	gtk_container_add (GTK_CONTAINER (bbox),   add);
	gtk_container_add (GTK_CONTAINER (bbox),   remove);
	gtk_container_add (GTK_CONTAINER (bbox),   close);

	gtk_signal_connect (GTK_OBJECT (add),    "clicked", (GtkSignalFunc) add_text,    NULL);
	gtk_signal_connect (GTK_OBJECT (remove), "clicked", (GtkSignalFunc) remove_text, NULL);
	gtk_signal_connect (GTK_OBJECT (close),  "clicked", (GtkSignalFunc) destroy,     NULL);
	gtk_signal_connect (GTK_OBJECT (app),    "destroy", (GtkSignalFunc) destroy,     NULL);
	
	gtk_window_set_default_size (GTK_WINDOW (app), 0, 300);

	gtk_widget_set_sensitive (remove, FALSE);
	gtk_widget_grab_focus	 (text);

	gtk_widget_show_all (app);
	gtk_main();
	
	return 0;
}

