#include <gnome.h>
#include "compare.h"
#include "spawn.h"

#define COMPARE_APPNAME "compare"
#define COMPARE_WINNAME "Compare files"
#define COMPARE_VERSION "0.0.1"

#define COMPARE_LEFT	"left:\t"
#define COMPARE_RIGHT	"rght:\t"
#define COMPARE_SAME	"same:\t"

/*
diff	--old-line-format='left: %L'		\
	--unchanged-line-format='same: %L'	\
	--new-line-format='rght: %L'		\
	read1.c read2.c
*/

void
cdestroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

int
compare (char *left, char *right)
{
	char buffer [1024];
	char number [10];
	char *cols[] = { "line no.", "left", "right" };
	char *text[3] = { number, NULL, NULL };
	int line = 1;
	GtkWidget *app = NULL;
	FILE *f = NULL;
	GtkWidget *clist = NULL;
	GtkWidget *scroll = NULL;
	GtkStyle *style_left = NULL;
	GtkStyle *style_right = NULL;
	GtkStyle *style_same = NULL;
	GtkStyle *style_none = NULL;
	GtkStyle *style_disc = NULL;
	//GtkStyle *style_col1 = NULL;
	GtkStyle *style_col2 = NULL;
	GtkStyle *style_col3 = NULL;
	int row;
	GdkColor red    = { 0, 65535, 60000, 60000 };
	GdkColor yellow = { 0, 65535, 65535, 50000 };
	//GdkColor green  = { 0,     0, 65535,     0 };
	//GdkColor blue   = { 0,     0,     0, 65535 };
	GdkColor black  = { 0,     0,     0,     0 };
	GdkColor white  = { 0, 65535, 65535, 65535 };
	GdkColor grey   = { 0, 60000, 60000, 60000 };
	gboolean one_pane = TRUE;
	int left_count = 0;
	int right_count = 0;
	char *cmdline = NULL;

	app = gnome_app_new (COMPARE_APPNAME, COMPARE_WINNAME);
	gtk_window_set_default_size (GTK_WINDOW (app), 500, 700);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", (GtkSignalFunc) cdestroy, NULL);

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

	cmdline = g_strdup_printf ("diff"
				   " --old-line-format="       COMPARE_LEFT  "%%L"
				   " --unchanged-line-format=" COMPARE_SAME  "%%L"
				   " --new-line-format="       COMPARE_RIGHT "%%L"
				   " %s %s", left, right);
	//g_print ("cmdline = %s\n", cmdline);
	f = run_diff (cmdline);

	gtk_clist_freeze (GTK_CLIST (clist));

	style_left  = gtk_style_new();
	style_right = gtk_style_new();
	style_same  = gtk_style_new();
	style_none  = gtk_style_new();
	style_disc  = gtk_style_new();

	gdk_font_unref (style_left ->font);
	gdk_font_unref (style_right->font);
	gdk_font_unref (style_same ->font);

	style_left ->font = gdk_font_load ("-bitstream-courier-medium-r-normal-*-*-120-*-*-m-*-iso8859-1");
	style_right->font = gdk_font_load ("-bitstream-courier-medium-r-normal-*-*-120-*-*-m-*-iso8859-1");
	style_same ->font = gdk_font_load ("-bitstream-courier-medium-r-normal-*-*-120-*-*-m-*-iso8859-1");

	style_left ->fg[GTK_STATE_NORMAL] = black;
	style_same ->fg[GTK_STATE_NORMAL] = black;
	style_right->fg[GTK_STATE_NORMAL] = black;
	style_none ->fg[GTK_STATE_NORMAL] = black;
	style_disc ->fg[GTK_STATE_NORMAL] = white;

	style_left ->base[GTK_STATE_NORMAL] = yellow;
	style_same ->base[GTK_STATE_NORMAL] = white;
	style_right->base[GTK_STATE_NORMAL] = red;
	style_none ->base[GTK_STATE_NORMAL] = grey;
	style_disc ->base[GTK_STATE_NORMAL] = white;

	/*
	for (i = 0; i < 5; i++)
	{
		style_none->fg[i] = black;
		style_none->bg[i] = white;
		//style_none->light[i] = blue;
		//style_none->dark[i] = blue;
		//style_none->mid[i] = blue;
		//style_none->text[i] = blue;
		style_none->base[i] = white;
	}
	*/
	//style_none->black = blue;
	//style_none->white = blue;

	//gtk_widget_set_style (clist, style_none);

	gtk_clist_set_column_visibility (GTK_CLIST (clist), 0, FALSE);
	gtk_clist_set_column_visibility (GTK_CLIST (clist), 2, FALSE);

	while (fgets (buffer, sizeof (buffer), f))
	{
		//g_print ("buffer = %s", buffer);
		sprintf (number, "%d", line);

		if (strncmp (buffer, COMPARE_LEFT, 6) == 0)
		{
			text[1] = buffer + 6;
			text[2] = "";
			style_col2 = style_left;
			style_col3 = style_disc;

			row = gtk_clist_append (GTK_CLIST (clist), text);
			gtk_clist_set_cell_style (GTK_CLIST (clist), row, 1, style_col2);
			gtk_clist_set_cell_style (GTK_CLIST (clist), row, 2, style_col3);

			left_count++;
		}
		else if (strncmp (buffer, COMPARE_RIGHT, 6) == 0)
		{
			if (one_pane)
			{
				text[1] = buffer + 6;
				text[2] = "";
			}
			else
			{
				text[1] = "";
				text[2] = buffer + 6;
			}
			style_col2 = style_disc;
			style_col3 = style_right;

			if ((right_count <= left_count) && (left_count != 0) && (!one_pane))
			{
				//row = gtk_clist_append (GTK_CLIST (clist), text);
				gtk_clist_set_text (GTK_CLIST (clist), row - left_count + right_count + 1, 2, buffer + 6);
				//gtk_clist_set_cell_style (GTK_CLIST (clist), row, 1, style_col2);
				gtk_clist_set_cell_style (GTK_CLIST (clist), row -left_count + right_count + 1, 2, style_col3);
			}
			else
			{
				row = gtk_clist_append (GTK_CLIST (clist), text);
				if (one_pane)
				{
					gtk_clist_set_cell_style (GTK_CLIST (clist), row, 1, style_col3);
					gtk_clist_set_cell_style (GTK_CLIST (clist), row, 2, style_col2);
				}
				else
				{
					gtk_clist_set_cell_style (GTK_CLIST (clist), row, 1, style_col2);
					gtk_clist_set_cell_style (GTK_CLIST (clist), row, 2, style_col3);
				}
			}

			right_count++;
		}
		else if (strncmp (buffer, COMPARE_SAME, 6) == 0)
		{
			text[1] = buffer + 6;
			text[2] = buffer + 6;
			style_col2 = style_same;
			style_col3 = style_same;

			row = gtk_clist_append (GTK_CLIST (clist), text);
			gtk_clist_set_cell_style (GTK_CLIST (clist), row, 1, style_col2);
			gtk_clist_set_cell_style (GTK_CLIST (clist), row, 2, style_col3);

			left_count = 0;
			right_count = 0;
		}

		gtk_clist_set_cell_style (GTK_CLIST (clist), row, 0, style_none);
		line++;
	}

	//gtk_clist_set_shadow_type (GTK_CLIST (clist), GTK_SHADOW_NONE);

	gtk_clist_columns_autosize (GTK_CLIST (clist));
	gtk_clist_thaw             (GTK_CLIST (clist));

	fclose (f);
	gtk_main();

	return 0;
}

