/*
    compare.c -- description
    Copyright (C) 1999  Richard Russon <richard.russon@ait.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Revision: 1.20 $ */

#include <gnome.h>
#include "compare.h"
#include "spawn.h"

// These are arbitrary, as long as they're different
#define COMPARE_LEFT	"left:\t"
#define COMPARE_RIGHT	"rght:\t"
#define COMPARE_SAME	"same:\t"

/*
diff	--old-line-format='left: %L'		\
	--unchanged-line-format='same: %L'	\
	--new-line-format='rght: %L'		\
	file1.c file2.c
*/

//XXX finalize?

/*----------------------------------------------------------------------------*/
static void gtk_compare (GtkCompare *compare);
static void gtk_compare_class_init	 (GtkCompareClass * klass);
static void gtk_compare_draw (GtkWidget *widget, GdkRectangle *area);
static void gtk_compare_init		 (GtkCompare * compare);
static void gtk_compare_realize (GtkWidget *widget);
static void gtk_compare_show (GtkWidget *widget);

GtkWidget * gtk_compare_new (DiffOptions *diff);
guint gtk_compare_get_type (void);
void gtk_compare_class_init (GtkCompareClass * klass);
void gtk_compare_init (GtkCompare * compare);
/*----------------------------------------------------------------------------*/

static void (* old_show_handler)    (GtkWidget *widget)                        = NULL;
static void (* old_realize_handler) (GtkWidget *widget)                        = NULL;
static void (* old_draw_handler)    (GtkWidget *widget, GdkRectangle *area)    = NULL;

static void
gtk_compare_show (GtkWidget *widget)
{
	g_print ("gtk_compare_show\n");
	old_show_handler (widget);
}

static void
gtk_compare_realize (GtkWidget *widget)
{
	g_print ("gtk_compare_realize\n");
	old_realize_handler (widget);
}

static void
gtk_compare_draw (GtkWidget *widget, GdkRectangle *area)
{
	GtkCompare *compare  = GTK_COMPARE (widget);

	g_print ("gtk_compare_draw\n");

	old_draw_handler (widget, area);

	if (compare->drawn == FALSE)
	{
		compare->drawn = TRUE;
		gtk_compare (compare);
	}
}

guint
gtk_compare_get_type (void)
{
	static guint compare_type = 0;
	static const GtkTypeInfo compare_info =
	{
		"GtkCompare",
		sizeof (GtkCompare),
		sizeof (GtkCompareClass),
		(GtkClassInitFunc)  gtk_compare_class_init,
		(GtkObjectInitFunc) gtk_compare_init,
		/* reserved */ NULL,
		/* reserved */ NULL,
		(GtkClassInitFunc) NULL,
	};

	//g_print ("gtk_compare_get_type\n");
	if (!compare_type)
	{
		// has to be 'derived' from clist to be able to cast to it!
		compare_type = gtk_type_unique (gtk_clist_get_type(), &compare_info);
	}

	return compare_type;
}

void
gtk_compare_init (GtkCompare * compare)
{
	compare->diff = NULL;
	compare->mdi_child = NULL;
	compare->flag1 = 0;
	compare->drawn = FALSE;
}

void
gtk_compare_class_init (GtkCompareClass * klass)
{
	GtkWidgetClass *widget_class = NULL;

	g_return_if_fail (klass != NULL);

	//g_print ("gtk_compare_class_init\n");

	// override methods
	widget_class = (GtkWidgetClass*) klass;

	old_show_handler   = widget_class->show;
	old_realize_handler= widget_class->realize;
	old_draw_handler   = widget_class->draw;

	widget_class->show               = gtk_compare_show;
	widget_class->draw               = gtk_compare_draw;
	widget_class->realize            = gtk_compare_realize;
}

GtkWidget *
gtk_compare_new (DiffOptions *diff)
{
	char *text[1] = { "Please wait..." };
	GtkWidget *widget = NULL;
	GtkCList  *list	  = NULL;
	GtkCompare *compare = NULL;
	int columns = 1;

	//g_print ("gtk_compare_new\n");
	widget = gtk_widget_new (GTK_TYPE_COMPARE,
				 "n_columns",   columns,
				 NULL);

	g_return_val_if_fail (widget != NULL, NULL);

	list = GTK_CLIST (widget);
	compare = GTK_COMPARE (widget);

	list->columns     = columns;
	compare->diff = diff;

	gtk_clist_append (list, text);

	//gtk_clist_construct (list, columns, NULL);// alread constructed
	/*
	if (titles)
	{
		int i;
		for (i = 0; i < columns; i++)
		{
			//g_print ("col %d, title %s\n", i, titles[i]);
			gtk_clist_set_column_title (list, i, titles[i]);
		}

		gtk_clist_column_titles_show (list);
		//g_print ("row = %p, cell = %p\n", list->row_mem_chunk, list->cell_mem_chunk);
	}
	*/

	return widget;
}

static void
gtk_compare (GtkCompare *compare)
{
	char buffer [1024];
	char number [10];
	char *text[3] = { number, NULL, NULL };
	int line = 1;
	FILE *f = NULL;
	GtkCList *clist = NULL;
	GtkStyle *style_left = NULL;
	GtkStyle *style_right = NULL;
	GtkStyle *style_same = NULL;
	GtkStyle *style_none = NULL;
	GtkStyle *style_disc = NULL;
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

	clist = GTK_CLIST (compare);

	//XXX this should be in the initialise (once off)
	gtk_clist_set_selection_mode    (clist, GTK_SELECTION_BROWSE);
	gtk_clist_column_titles_passive (clist);

	//gtk_widget_show_all (scroll);
	while (gtk_events_pending ())
		gtk_main_iteration();

	//sleep (5); //XXX
	cmdline = g_strdup_printf ("diff"
				   " --old-line-format="       COMPARE_LEFT  "%%L"
				   " --unchanged-line-format=" COMPARE_SAME  "%%L"
				   " --new-line-format="       COMPARE_RIGHT "%%L"
				   " %s %s", compare->diff->left, compare->diff->right);
	//g_print ("cmdline = %s\n", cmdline);
	f = run_diff (cmdline);

	while (gtk_events_pending ())
		gtk_main_iteration();

	gtk_clist_freeze (clist);
	gtk_clist_clear  (clist);

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

	gtk_clist_set_column_visibility (clist, 0, FALSE);
	gtk_clist_set_column_visibility (clist, 2, FALSE);

	while (fgets (buffer, sizeof (buffer), f))
	{
		//g_print ("buffer = %s", buffer);
		sprintf (number, "%d", line);

		if (strncmp (buffer, COMPARE_LEFT, 6) == 0)
		{
			text[0] = buffer + 6;
			//text[1] = buffer + 6;
			//text[2] = "";
			style_col2 = style_left;
			style_col3 = style_disc;

			row = gtk_clist_append (clist, text);
			//gtk_clist_set_cell_style (clist, row, 1, style_col2);
			//gtk_clist_set_cell_style (clist, row, 2, style_col3);
			gtk_clist_set_cell_style (clist, row, 0, style_col2);

			left_count++;
		}
		else if (strncmp (buffer, COMPARE_RIGHT, 6) == 0)
		{
			if (one_pane)
			{
				text[0] = buffer + 6;
				//text[1] = buffer + 6;
				//text[2] = "";
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
				//row = gtk_clist_append (clist, text);
				gtk_clist_set_text (clist, row - left_count + right_count + 1, 2, buffer + 6);
				//gtk_clist_set_cell_style (clist, row, 1, style_col2);
				gtk_clist_set_cell_style (clist, row -left_count + right_count + 1, 2, style_col3);
			}
			else
			{
				row = gtk_clist_append (clist, text);
				if (one_pane)
				{
					gtk_clist_set_cell_style (clist, row, 0, style_col3);
					//gtk_clist_set_cell_style (clist, row, 1, style_col3);
					//gtk_clist_set_cell_style (clist, row, 2, style_col2);
				}
				else
				{
					gtk_clist_set_cell_style (clist, row, 1, style_col2);
					gtk_clist_set_cell_style (clist, row, 2, style_col3);
				}
			}

			right_count++;
		}
		else if (strncmp (buffer, COMPARE_SAME, 6) == 0)
		{
			text[0] = buffer + 6;
			//text[1] = buffer + 6;
			//text[2] = buffer + 6;
			style_col2 = style_same;
			style_col3 = style_same;

			row = gtk_clist_append (clist, text);
			gtk_clist_set_cell_style (clist, row, 0, style_col2);
			//gtk_clist_set_cell_style (clist, row, 1, style_col2);
			//gtk_clist_set_cell_style (clist, row, 2, style_col3);

			left_count = 0;
			right_count = 0;
		}

		//gtk_clist_set_cell_style (clist, row, 0, style_none);
		line++;
	}

	fclose (f);

	//gtk_clist_set_shadow_type (clist, GTK_SHADOW_NONE);
	gtk_clist_columns_autosize (clist);
	gtk_clist_thaw             (clist);

	while (gtk_events_pending ())
		gtk_main_iteration();

	//return scroll;
}

