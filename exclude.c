#include <gnome.h>

#define X_APPNAME "exclude"
#define X_WINNAME "Exclude"
#define X_VERSION "0.0.1"

static GtkWidget *app		= NULL;
static GtkWidget *scroll	= NULL;
static GtkWidget *table		= NULL;
static GtkWidget *clist		= NULL;
static GtkWidget *bbox		= NULL;
static GtkWidget *text		= NULL;
static GtkWidget *add		= NULL;
static GtkWidget *bremove	= NULL;
static GtkWidget *bclose		= NULL;
static GtkAccelGroup *accel    	= NULL;

int selection = -1;

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

void
select_row (GtkCList *clist, gint row, gint column, GdkEvent *event)
{
	g_print ("selected row %d\n", row);
	gtk_widget_set_sensitive (bremove, TRUE);
}

void
unselect_row (GtkCList *clist, gint row, gint column, GdkEvent *event)
{
	g_print ("unselected row %d\n", row);
	gtk_widget_set_sensitive (bremove, FALSE);
}

int
main (int argc, char *argv[])
{
	gnome_init (X_APPNAME, X_VERSION, argc, argv);

	app	= gnome_app_new (X_APPNAME, X_WINNAME);
	table	= gtk_table_new (3, 1, FALSE);
	scroll	= gtk_scrolled_window_new (NULL, NULL);
	clist	= gtk_clist_new (2);
	text	= gtk_entry_new();
	bbox	= gtk_hbutton_box_new();
	add   	= gtk_button_new_with_label ("Add");
	bremove	= gtk_button_new_with_label ("Remove");
	bclose 	= gtk_button_new_with_label ("Close");

	gtk_label_set_pattern (GTK_LABEL (GTK_BIN (add)   ->child), "_");
	gtk_label_set_pattern (GTK_LABEL (GTK_BIN (bremove)->child), "_");
	gtk_label_set_pattern (GTK_LABEL (GTK_BIN (bclose) ->child), "_");

	accel = gtk_accel_group_new ();

	gtk_accel_group_attach (accel, GTK_OBJECT (app));

	gtk_widget_add_accelerator (add,    "clicked", accel, GDK_a, GDK_MOD1_MASK, 0);
	gtk_widget_add_accelerator (bremove, "clicked", accel, GDK_r, GDK_MOD1_MASK, 0);
	gtk_widget_add_accelerator (bclose,  "clicked", accel, GDK_c, GDK_MOD1_MASK, 0);

	gtk_button_box_set_layout     (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing    (GTK_BUTTON_BOX (bbox), 10);

	gnome_app_set_contents (GNOME_APP (app), table);

	gtk_table_attach (GTK_TABLE (table), scroll, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND | GTK_SHRINK, GTK_FILL | GTK_EXPAND | GTK_SHRINK, 10, 10);
	gtk_table_attach (GTK_TABLE (table), text,   0, 1, 1, 2, GTK_FILL, GTK_FILL, 10, 10);
	gtk_table_attach (GTK_TABLE (table), bbox,   0, 1, 2, 3, GTK_FILL, GTK_FILL, 5, 5);

	gtk_container_add (GTK_CONTAINER (scroll), clist);
	gtk_container_add (GTK_CONTAINER (bbox),   add);
	gtk_container_add (GTK_CONTAINER (bbox),   bremove);
	gtk_container_add (GTK_CONTAINER (bbox),   bclose);

	gtk_signal_connect (GTK_OBJECT (add),    "clicked",    (GtkSignalFunc) add_text,    NULL);
	gtk_signal_connect (GTK_OBJECT (bremove), "clicked",    (GtkSignalFunc) remove_text, NULL);
	gtk_signal_connect (GTK_OBJECT (bclose),  "clicked",    (GtkSignalFunc) destroy,     NULL);
	gtk_signal_connect (GTK_OBJECT (app),    "destroy",    (GtkSignalFunc) destroy,     NULL);
	gtk_signal_connect (GTK_OBJECT (clist),  "select_row", (GtkSignalFunc) select_row,  NULL);
	gtk_signal_connect (GTK_OBJECT (clist),  "unselect_row", (GtkSignalFunc) unselect_row,  NULL);
	
	gtk_window_set_default_size (GTK_WINDOW (app), 0, 300);

	gtk_widget_set_sensitive (bremove, FALSE);
	gtk_widget_grab_focus	 (text);

	gtk_widget_show_all (app);
	gtk_main();
	
	return 0;
}

