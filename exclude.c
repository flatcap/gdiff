#include <gnome.h>

#define X_APPNAME "exclude"
#define X_WINNAME "Exclude"
#define X_VERSION "0.0.1"

static GtkWidget *app		= NULL;
static GtkWidget *scroll	= NULL;
static GtkWidget *table		= NULL;
static GtkWidget *list		= NULL;
static GtkWidget *bbox		= NULL;
static GtkWidget *text		= NULL;
static GtkWidget *add		= NULL;
static GtkWidget *bremove	= NULL;
static GtkWidget *bclose		= NULL;
static GtkAccelGroup *accel    	= NULL;

int selection = -1;

void
destroy (GtkWidget *widget, gpointer data)
{
	gtk_exit (0);
}

void
add_text (GtkWidget *widget, gpointer data)
{
	GtkWidget *check = NULL;
	GtkWidget *item  = NULL;
	GString *str = g_string_new (gtk_entry_get_text (GTK_ENTRY (text)));

	gtk_entry_set_text (GTK_ENTRY (text), "");

	item  = gtk_list_item_new();
	check = gtk_check_button_new_with_label (str->str);

	gtk_container_add (GTK_CONTAINER (item), check);
	gtk_container_add (GTK_CONTAINER (list), item);

	gtk_widget_show_all (item);
}

void
remove_text (GtkWidget *widget, gpointer data)
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

/* Turn "_Jim" into a button called "Jim" that is accelerated by Alt-J */
GtkWidget*
gtk_button_new_with_accel (const gchar *raw_label, GtkAccelGroup *accel)
{
	GtkWidget *button     = NULL;
	GString   *label      = NULL;
	GString   *pattern    = NULL;
	gchar     *underscore = NULL;
	gchar      mnemonic   = 0;
	gint       offset     = 0;

	g_return_val_if_fail (raw_label != NULL, NULL);
	g_return_val_if_fail (accel     != NULL, NULL);

	label      = g_string_new (raw_label);
	underscore = strchr       (raw_label, '_');

	if (underscore)
	{
		mnemonic = *(underscore+1);			// Could still be 0

		offset = (underscore - raw_label);
		g_string_erase (label, offset, 1);
		pattern = g_string_new (g_strnfill (offset, ' '));
		g_string_append_c (pattern, '_');
	}

	button = gtk_button_new_with_label (label->str);
	
	g_string_free (label, TRUE);

	if (pattern && button)
	{
		gtk_label_set_pattern (GTK_LABEL (GTK_BIN (button)->child), pattern->str);
		g_string_free (pattern, TRUE);
	}

	if (mnemonic)
	{
		gtk_widget_add_accelerator (button, "clicked", accel, mnemonic, GDK_MOD1_MASK, 0);
	}

	return button;
}

int
main (int argc, char *argv[])
{
	//static char *cols[] = { "col1", "col2" };
	gnome_init (X_APPNAME, X_VERSION, argc, argv);

	app	= gnome_app_new (X_APPNAME, X_WINNAME);
	table	= gtk_table_new (3, 1, FALSE);
	scroll	= gtk_scrolled_window_new (NULL, NULL);
	//clist	= gtk_clist_new_with_titles (2, cols);
	list	= gtk_list_new();
	text	= gtk_entry_new();
	bbox	= gtk_hbutton_box_new();
	accel   = gtk_accel_group_new ();

	add   	= gtk_button_new_with_accel ("_Add",    accel);
	bremove	= gtk_button_new_with_accel ("_Remove", accel);
	bclose 	= gtk_button_new_with_accel ("_Close",  accel);

	GTK_WIDGET_SET_FLAGS (add,     GTK_CAN_DEFAULT);
	GTK_WIDGET_SET_FLAGS (bremove, GTK_CAN_DEFAULT);
	GTK_WIDGET_SET_FLAGS (bclose,  GTK_CAN_DEFAULT);

	gtk_accel_group_attach (accel, GTK_OBJECT (app));

	gtk_button_box_set_layout     (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing    (GTK_BUTTON_BOX (bbox), 10);

	gnome_app_set_contents (GNOME_APP (app), table);

	//gtk_container_add (GTK_CONTAINER (scroll), list);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scroll), list);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

	gtk_container_add (GTK_CONTAINER (bbox),   add);
	gtk_container_add (GTK_CONTAINER (bbox),   bremove);
	gtk_container_add (GTK_CONTAINER (bbox),   bclose);

	gtk_table_attach (GTK_TABLE (table), scroll, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND | GTK_SHRINK, GTK_FILL | GTK_EXPAND | GTK_SHRINK, 10, 10);
	gtk_table_attach (GTK_TABLE (table), text,   0, 1, 1, 2, GTK_FILL, GTK_FILL, 10, 10);
	gtk_table_attach (GTK_TABLE (table), bbox,   0, 1, 2, 3, GTK_FILL, GTK_FILL, 5, 5);

	gtk_signal_connect (GTK_OBJECT (add),    "clicked",    (GtkSignalFunc) add_text,    NULL);
	gtk_signal_connect (GTK_OBJECT (bremove), "clicked",    (GtkSignalFunc) remove_text, NULL);
	gtk_signal_connect (GTK_OBJECT (bclose),  "clicked",    (GtkSignalFunc) destroy,     NULL);
	gtk_signal_connect (GTK_OBJECT (app),    "destroy",    (GtkSignalFunc) destroy,     NULL);
	//gtk_signal_connect (GTK_OBJECT (list),  "select_row", (GtkSignalFunc) select_row,  NULL);
	//gtk_signal_connect (GTK_OBJECT (list),  "unselect_row", (GtkSignalFunc) unselect_row,  NULL);
	
	gtk_signal_connect_object (GTK_OBJECT (text), "activate",
				  (GtkSignalFunc) gtk_button_clicked, GTK_OBJECT (add));

	gtk_window_set_default_size (GTK_WINDOW (app), 0, 300);

	//gtk_clist_set_selection_mode (GTK_CLIST (clist), GTK_SELECTION_BROWSE);
	gtk_widget_set_sensitive     (bremove, FALSE);
	gtk_widget_grab_focus	     (text);
	gtk_widget_grab_default      (add);

	gtk_widget_show_all (app);
	gtk_main();
	
	return 0;
}

