#include <gnome.h>

#define COMPARE_APPNAME "compare"
#define COMPARE_WINNAME "Compare files"
#define COMPARE_VERSION "0.0.1"

void
destroy (GtkWidget *widget, gpointer *data)
{
	gtk_exit (0);
}

int
main (int argc, char *argv[])
{
	char buffer [1024];
	char left [1024];
	char right [1024];
	char number [10];
	char *cols[] = { "line no.", "left", "right" };
	char *text[3] = { number, left, right };
	int line = 1;
	GtkWidget *app = NULL;
	FILE *f = NULL;
	GtkWidget *clist = NULL;
	GtkWidget *scroll = NULL;
	GtkStyle *style = NULL;
	int row;

	gnome_init (COMPARE_APPNAME, COMPARE_VERSION, argc, argv);

	app = gnome_app_new (COMPARE_APPNAME, COMPARE_WINNAME);
	gtk_window_set_default_size (GTK_WINDOW (app), 500, 700);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", (GtkSignalFunc) destroy, NULL);

	clist = gtk_clist_new_with_titles (3, cols);

	gtk_clist_set_selection_mode    (GTK_CLIST (clist), GTK_SELECTION_BROWSE);
	gtk_clist_column_titles_passive (GTK_CLIST (clist));

	scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER(scroll), clist);

	gnome_app_set_contents (GNOME_APP (app), scroll);

	gtk_widget_show_all (app);
	while (gtk_events_pending ())
		gtk_main_iteration();

	f = stdin;

	gtk_clist_freeze (GTK_CLIST (clist));

	style = gtk_style_new();
	gdk_font_unref (style->font);
	style->font = gdk_font_load ("-bitstream-courier-medium-r-normal-*-*-120-*-*-m-*-iso8859-1");
	
	while (fgets (buffer, sizeof (buffer), f))
	{
		memset (left, 0, sizeof (left));
		memset (right, 0, sizeof (right));
		memset (number, 0, sizeof (number));
		text[1] = g_strchomp (left);
		text[2] = g_strchomp (right);

		sprintf (number, "%d", line);
		strncpy (left, buffer, 128);		// -W 260
		strncpy (right, buffer + 132, 128);

		row = gtk_clist_append (GTK_CLIST (clist), text);
		gtk_clist_set_cell_style (GTK_CLIST (clist), row, 1, style);
		gtk_clist_set_cell_style (GTK_CLIST (clist), row, 2, style);

		//g_print ("%s", buffer);

		line++;
	}

	gtk_clist_columns_autosize      (GTK_CLIST (clist));
	gtk_clist_thaw (GTK_CLIST (clist));

	// fclose (f);
	gtk_main();

	return 0;
}
