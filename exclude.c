/*
    exclude.c -- description
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

#include <gnome.h>

#define X_APPNAME "exclude"
#define X_WINNAME "Exclude"
#define X_VERSION "0.0.1"

static GtkAccelGroup *accel    	= NULL;
static GtkWidget *add		= NULL;
static GtkWidget *app		= NULL;
static GtkWidget *bbox		= NULL;
static GtkWidget *bclose	= NULL;
static GtkWidget *bremove	= NULL;
static GtkWidget *list		= NULL;
static GtkWidget *scroll	= NULL;
static GtkWidget *text		= NULL;
static GtkWidget *vbox		= NULL;

int selection = -1;

void
exclude_destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

void
add_text (GtkWidget *widget, gpointer data)
{
	GtkWidget       *check  = NULL;
	GtkToggleButton *toggle = NULL;
	GtkWidget       *label  = NULL;
	GtkWidget       *hbox   = NULL;
	GtkWidget       *item   = NULL;
	GString         *str    = g_string_new (gtk_entry_get_text (GTK_ENTRY (text)));

	if (strlen (str->str) == 0)
	{
		return;
	}

	gtk_entry_set_text (GTK_ENTRY (text), "");

	item   = gtk_list_item_new();
	check  = gtk_check_button_new();
	label  = gtk_label_new (str->str);
	hbox   = gtk_hbox_new (FALSE, 0);
	toggle = GTK_TOGGLE_BUTTON (check);

	GTK_WIDGET_SET_FLAGS (toggle, GTK_CAN_DEFAULT);
	toggle->active = TRUE;

	gtk_box_pack_start (GTK_BOX (hbox), check, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE,  0);

	gtk_container_add (GTK_CONTAINER (item), hbox);
	gtk_container_add (GTK_CONTAINER (list), item);

	gtk_widget_show_all (item);
}

void
remove_text (GtkWidget *widget, gpointer data)
{
	g_print ("remove\n");
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

void
selection_changed (GtkWidget *widget, gpointer data)
{
	g_print ("selection_changed\n");
}

void
select_child (GtkWidget *widget, gpointer data)
{
	g_print ("select_child %s\n", gtk_widget_get_name (widget));
	gtk_widget_set_sensitive (bremove, TRUE);
}

void
unselect_child (GtkWidget *widget, gpointer data)
{
	g_print ("unselect_child\n");
	gtk_widget_set_sensitive (bremove, FALSE);
}

void
no_default (GtkWidget *widget, gpointer data)
{
	//GTK_WIDGET_UNSET_FLAGS (add, GTK_HAS_DEFAULT);
}

void
exclude_dialog (void)
{
	app	= gnome_app_new (X_APPNAME, X_WINNAME);
	vbox	= gtk_vbox_new (FALSE, 5);
	scroll	= gtk_scrolled_window_new (NULL, NULL);
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

	gnome_app_set_contents (GNOME_APP (app), vbox);

	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scroll), list);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

	gtk_container_add (GTK_CONTAINER (bbox),   add);
	gtk_container_add (GTK_CONTAINER (bbox),   bremove);
	gtk_container_add (GTK_CONTAINER (bbox),   bclose);

	gtk_box_pack_start (GTK_BOX (vbox), scroll, TRUE,  TRUE,  0);
	gtk_box_pack_start (GTK_BOX (vbox), text,   FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), bbox,   FALSE, FALSE, 0);

	gtk_signal_connect (GTK_OBJECT (add),    "clicked",    (GtkSignalFunc) add_text,    NULL);
	gtk_signal_connect (GTK_OBJECT (bremove), "clicked",    (GtkSignalFunc) remove_text, NULL);
	gtk_signal_connect (GTK_OBJECT (bclose),  "clicked",    (GtkSignalFunc) exclude_destroy,     NULL);
	gtk_signal_connect (GTK_OBJECT (app),    "destroy",    (GtkSignalFunc) exclude_destroy,     NULL);
	gtk_signal_connect (GTK_OBJECT (list),  "selection_changed", (GtkSignalFunc) selection_changed,  NULL);
	gtk_signal_connect (GTK_OBJECT (list),  "select_child", (GtkSignalFunc) select_child,  NULL);
	gtk_signal_connect (GTK_OBJECT (list),  "unselect_child", (GtkSignalFunc) unselect_child,  NULL);
	
	gtk_signal_connect_object (GTK_OBJECT (text), "focus_in_event",
				   (GtkSignalFunc) gtk_widget_grab_default,
				   GTK_OBJECT (add));

	/*
	gtk_signal_connect_object (GTK_OBJECT (text), "focus_in_event",
				   (GtkSignalFunc) gtk_widget_grab_default,
				   GTK_OBJECT (add));
	*/

	//gtk_signal_connect (GTK_OBJECT (text), "focus_out_event", (GtkSignalFunc) no_default, add);

	gtk_signal_connect_object (GTK_OBJECT (text), "activate",
				  (GtkSignalFunc) gtk_button_clicked, GTK_OBJECT (add));

	gtk_window_set_default_size (GTK_WINDOW (app), 0, 300);

	gtk_list_set_selection_mode  (GTK_LIST (list), GTK_SELECTION_BROWSE);
	gtk_widget_set_sensitive     (bremove, FALSE);
	gtk_widget_grab_focus	     (text);
	gtk_widget_grab_default      (add);

	gtk_widget_show_all (app);
	gtk_main();
}

