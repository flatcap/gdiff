/*
    style.c -- description
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

/* $Revision: 1.23 $ */

#include "style.h"

void
style_page_list_cb (GtkWidget *widget, gpointer data)
{
}

void
style_page_fg_cb (GtkWidget *widget, gpointer data)
{
}

void
style_page_bg_cb (GtkWidget *widget, gpointer data)
{
}

void
style_page_fg_colour_cb (GtkWidget *widget, gpointer data)
{
}

void
style_page_bg_colour_cb (GtkWidget *widget, gpointer data)
{
}

//XXX move this an exclusions to their own files entirely
void
add_style_page (GnomePropertyBox *props)
{
	static char *text[] = { "List Same", "List Left", "List Right", "List Diff", "List Error / Type", "File Left", "File Right", "Overview Left", "Overview Right", NULL };
	GtkWidget *frame     = gtk_frame_new ("Style");
	GtkWidget *padding   = gtk_vbox_new  (FALSE, GNOME_PAD_SMALL);
	GtkWidget *label     = gtk_label_new ("Style");
	GtkWidget *hbox      = gtk_hbox_new  (FALSE, GNOME_PAD_SMALL);
	GtkWidget *clist     = gtk_clist_new (1);
	GtkWidget *vbox      = gtk_vbox_new  (FALSE, GNOME_PAD_SMALL);
	GtkWidget *entry     = gtk_entry_new ();
	GtkWidget *fg_box    = gtk_hbox_new  (FALSE, GNOME_PAD_SMALL);
	GtkWidget *bg_box    = gtk_hbox_new  (FALSE, GNOME_PAD_SMALL);
	GtkWidget *fg_check  = gtk_check_button_new_with_label ("Foreground");
	GtkWidget *bg_check  = gtk_check_button_new_with_label ("Background");
	GtkWidget *fg_dummy  = gtk_label_new ("");
	GtkWidget *bg_dummy  = gtk_label_new ("");
	GtkWidget *fg_colour = gnome_color_picker_new();
	GtkWidget *bg_colour = gnome_color_picker_new();
	int i;

	gtk_clist_set_selection_mode     (GTK_CLIST (clist), GTK_SELECTION_BROWSE);
	gtk_clist_set_column_auto_resize (GTK_CLIST (clist), 0, TRUE);
	for (i = 0; text[i]; i++)
	{
		gtk_clist_append (GTK_CLIST (clist), &text[i]);
	}

	gtk_entry_set_text (GTK_ENTRY (entry), _("Sample text"));
	gtk_widget_set_sensitive (entry, FALSE);

	gtk_box_pack_start (GTK_BOX (fg_box), fg_check,  FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (fg_box), fg_dummy,  TRUE,  TRUE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (fg_box), fg_colour, FALSE, FALSE, GNOME_PAD_SMALL);

	gtk_box_pack_start (GTK_BOX (bg_box), bg_check,  FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (bg_box), bg_dummy,  TRUE,  TRUE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (bg_box), bg_colour, FALSE, FALSE, GNOME_PAD_SMALL);

	gtk_box_pack_start (GTK_BOX (vbox), entry,  FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (vbox), fg_box, FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (vbox), bg_box, FALSE, FALSE, GNOME_PAD_SMALL);

	gtk_box_pack_start (GTK_BOX (hbox), clist, FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (hbox), vbox,  FALSE, FALSE, GNOME_PAD_SMALL);

	gtk_box_pack_start (GTK_BOX (padding), hbox, TRUE, TRUE, GNOME_PAD_SMALL);

	gtk_container_add  (GTK_CONTAINER (frame), padding);

	gnome_property_box_append_page (props, frame, label);
}


