/*
    file.c -- description
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

/* $Revision: 1.16 $ */

#include <gnome.h>
#include "file.h"
#include "options.h"
#include "diff.h"
#include "mdi.h"
#include "args.h"

// These struct are used by the callbacks to avoid global variables
typedef struct _BrowseInfo BrowseInfo;
typedef struct _FileInfo   FileInfo;

struct _BrowseInfo
{
	gboolean	 left;
	char		*title;
	GtkEntry	*entry;
	GtkWindow	*parent;
};

struct _FileInfo
{
	GnomeMDI	*mdi;
	GtkEntry	*lentry;
	GtkEntry	*rentry;
};

#if 0
	//XXX neat!
	gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "focus_in_event",
						(GtkSignalFunc) gtk_widget_grab_default,
						GTK_OBJECT (filesel->ok_button));
	gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "activate",
						(GtkSignalFunc) gtk_button_clicked,
						GTK_OBJECT (filesel->ok_button));
#endif

/*----------------------------------------------------------------------------*/
void new_file (GnomeMDI *mdi, GtkWindow *parent);

//XXX can we subdivide these into file stuff and browse stuff?
static void		browse_entry_changed	(GtkEntry *entry, GtkWidget *button);
static void		browse_filename		(GtkWidget *button, BrowseInfo *info);
static void		destroy_browse		(GtkWidget *parent, GtkWidget **browse);
static void		dialog_button_clicked	(GnomeDialog *dialog, gint button_number);
static void		dialog_destroyed	(GtkWidget *widget, gpointer data);
static void		entry_changed		(GtkEntry *entry, FileInfo *finfo);
static void		file_sel_ok		(GtkButton *button, BrowseInfo *info);
static GtkWidget *	new_file2		(GnomeMDI *mdi, GtkWindow *parent);
/*----------------------------------------------------------------------------*/

//______________________________________________________________________________
//
// Public
//

void
new_file (GnomeMDI *mdi, GtkWindow *parent)
{
	static GtkWidget *file = NULL;

	g_return_if_fail (parent != NULL);
	g_return_if_fail (mdi    != NULL);
	g_return_if_fail (GNOME_IS_MDI (mdi));

	if (file)
	{
		gdk_window_show  (file->window);
		gdk_window_raise (file->window);
	}
	else
	{
		file = new_file2 (mdi, parent);	//XXX internal (static)
		if (file)
		{
			gtk_signal_connect (GTK_OBJECT (file), "destroy", GTK_SIGNAL_FUNC (gtk_widget_destroyed), &file);
			gtk_widget_show_all (file);
		}
	}
}

//______________________________________________________________________________
//
// Private
//

static void
browse_entry_changed (GtkEntry *entry, GtkWidget *button)
{
	g_return_if_fail (entry  != NULL);
	g_return_if_fail (button != NULL);

	gtk_widget_set_sensitive (button, (strlen (gtk_entry_get_text (entry)) > 0));
}

static void
browse_filename (GtkWidget *button, BrowseInfo *info)
{
	static GtkWidget *lbrowse = NULL;
	static GtkWidget *rbrowse = NULL;

	GtkWidget **bptr = NULL;			// a pointer to one of the browsers

	g_return_if_fail (info != NULL);

	bptr = (info->left) ? (&lbrowse) : (&rbrowse);

	if (*bptr)
	{
		gdk_window_show  ((*bptr)->window);
		gdk_window_raise ((*bptr)->window);
	}
	else
	{
		(*bptr) = gtk_file_selection_new (info->title);
		if (*bptr)
		{
			// simplify the dialog / parent hides, we hide / parent dies, we die / button presses / text->OK
			gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (*bptr));
			gtk_widget_set_sensitive (GTK_FILE_SELECTION (*bptr)->ok_button, FALSE);

			gtk_window_set_transient_for (GTK_WINDOW (*bptr), info->parent);

			gtk_signal_connect (GTK_OBJECT (*bptr),        "destroy", GTK_SIGNAL_FUNC (gtk_widget_destroyed), bptr);
			gtk_signal_connect (GTK_OBJECT (info->parent), "destroy", GTK_SIGNAL_FUNC (destroy_browse),       bptr);

			gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (*bptr)->ok_button),     "clicked", GTK_SIGNAL_FUNC (file_sel_ok),    info);
			gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (*bptr)->cancel_button), "clicked", GTK_SIGNAL_FUNC (destroy_browse), bptr);

			gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (*bptr)->selection_entry), "changed", GTK_SIGNAL_FUNC (browse_entry_changed), GTK_FILE_SELECTION (*bptr)->ok_button);

			gtk_widget_show_all (*bptr);
		}
	}
}

static void
destroy_browse (GtkWidget *parent, GtkWidget **browse) //XXX misc.c?
{
	g_return_if_fail (browse != NULL);

	if (GTK_IS_WIDGET (*browse))
	{
		g_free (gtk_object_get_user_data (GTK_OBJECT (*browse)));	// BrowseInfo

		gtk_widget_destroy (*browse);
		*browse = NULL;
	}
}

static void
dialog_button_clicked (GnomeDialog *dialog, gint button_number)
{
	g_print ("dialog_button_clicked\n");
	switch (button_number)
	{
		case 0: show_preferences (GTK_WINDOW (dialog), PageDiff);	// Diff Options
			break;

		case 1:
		{
			FileInfo *finfo = gtk_object_get_user_data (GTK_OBJECT (dialog));
			DiffOptions *diff = diffoptions_new();

			diff->left = g_strdup (gtk_entry_get_text (finfo->lentry));
			diff->right = g_strdup (gtk_entry_get_text (finfo->rentry));
			categorise_args (diff);
			g_print ("%s,%s\n", diff->left, diff->right);
			g_print ("type  = %s\n", (diff->type == File) ? "file" : "dir");

			mdi_add_diff (finfo->mdi, diff);
			gnome_dialog_close (dialog);				// OK
			break;
		}
		case 2: gnome_dialog_close (dialog);				// Cancel
			break;

		default: g_assert_not_reached();
			break;
	}
}

static void
dialog_destroyed (GtkWidget *widget, gpointer data)
{
	g_return_if_fail (widget != NULL);

	g_print ("dialog_destroyed\n");

	g_free (gtk_object_get_user_data (GTK_OBJECT (widget)));	// FileInfo
}

static void
entry_changed (GtkEntry *entry, FileInfo *finfo)
{
	GtkWidget *dialog = NULL;
	gboolean data = FALSE;
	GtkWidget *button = NULL;

	g_return_if_fail (entry != NULL);

	dialog = gtk_widget_get_ancestor  (GTK_WIDGET (entry), gnome_dialog_get_type());

	button = GTK_WIDGET (g_list_nth_data (GNOME_DIALOG (dialog)->buttons, 1));	//XXX depends on buttons' order

	data = ((strlen (gtk_entry_get_text (finfo->lentry)) > 0) &&
		(strlen (gtk_entry_get_text (finfo->rentry)) > 0));

	gtk_widget_set_sensitive (button, data);
}

static void
file_sel_ok (GtkButton *button, BrowseInfo *info)
{
	GtkFileSelection *filesel = NULL;

	g_return_if_fail (button != NULL);
	g_return_if_fail (info   != NULL);

	filesel = GTK_FILE_SELECTION (gtk_widget_get_ancestor (GTK_WIDGET (button), gtk_file_selection_get_type()));

	g_return_if_fail (filesel != NULL);

	if (strlen (gtk_entry_get_text (GTK_ENTRY (filesel->selection_entry))) > 0)
	{
		gtk_entry_set_text (info->entry, gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel)));
		gtk_widget_destroy (GTK_WIDGET (filesel));
	}
}

static GtkWidget *
new_file2 (GnomeMDI *mdi, GtkWindow *parent)
{
	GtkWidget *dialog = NULL;
	GtkBox *box = NULL;
	GtkBox *lbox=NULL;
	GtkBox *rbox= NULL;
	GtkWidget *lentry = NULL;
	GtkWidget *rentry = NULL;
	GtkWidget *lbrowse = NULL;
	GtkWidget *rbrowse = NULL;
	GtkWidget *check   = NULL;
	BrowseInfo *lbinfo = NULL;
	BrowseInfo *rbinfo = NULL;
	FileInfo *finfo = NULL;

	lbinfo = g_malloc0 (sizeof (BrowseInfo));
	rbinfo = g_malloc0 (sizeof (BrowseInfo));
	finfo  = g_malloc0 (sizeof (FileInfo));

	lbinfo->title = "Select left file";
	rbinfo->title = "Select right file";
	lbinfo->left  = TRUE;
	rbinfo->left  = FALSE;

	dialog = gnome_dialog_new ("Select files or directories to be compared",
		"Diff options",
		GNOME_STOCK_BUTTON_OK,
		GNOME_STOCK_BUTTON_CANCEL,
		NULL);

	// if we're closed then the dialog NULLs its parent (us) pointer
	gtk_window_set_transient_for (GTK_WINDOW (dialog), parent);

	// disable the OK button, initially
	gtk_widget_set_sensitive (GTK_WIDGET (g_list_nth_data (GNOME_DIALOG (dialog)->buttons, 1)), FALSE);

	gtk_signal_connect (GTK_OBJECT (dialog), "clicked", dialog_button_clicked, NULL);
	gtk_signal_connect (GTK_OBJECT (dialog), "destroy", dialog_destroyed,      NULL);

	box = GTK_BOX ((GNOME_DIALOG (dialog))->vbox);

	lentry = gtk_entry_new();
	rentry = gtk_entry_new();

	finfo->mdi    = GNOME_MDI (mdi);
	finfo->lentry = GTK_ENTRY (lentry);
	finfo->rentry = GTK_ENTRY (rentry);

	lbinfo->entry = GTK_ENTRY (lentry);
	rbinfo->entry = GTK_ENTRY (rentry);

	lbinfo->parent = GTK_WINDOW (dialog);
	rbinfo->parent = GTK_WINDOW (dialog);

	gtk_signal_connect (GTK_OBJECT (lentry), "changed", entry_changed, finfo);
	gtk_signal_connect (GTK_OBJECT (rentry), "changed", entry_changed, finfo);

	lbox = GTK_BOX (gtk_hbox_new (FALSE, GNOME_PAD_SMALL));
	rbox = GTK_BOX (gtk_hbox_new (FALSE, GNOME_PAD_SMALL));

	lbrowse = gtk_button_new_with_label ("Browse...");
	rbrowse = gtk_button_new_with_label ("Browse...");

	gtk_signal_connect (GTK_OBJECT (lbrowse), "clicked", browse_filename, lbinfo);
	gtk_signal_connect (GTK_OBJECT (rbrowse), "clicked", browse_filename, rbinfo);

	// g_free'd in destroy_browse
	gtk_object_set_user_data (GTK_OBJECT (lbrowse), lbinfo);
	gtk_object_set_user_data (GTK_OBJECT (rbrowse), rbinfo);

	gtk_object_set_user_data (GTK_OBJECT (dialog), finfo);

	check = gtk_check_button_new_with_label ("Start browse automatically (Next time)");

	gtk_box_pack_start (box, GTK_WIDGET(lbox), FALSE, FALSE, 0);
	gtk_box_pack_start (box, GTK_WIDGET(rbox), FALSE, FALSE, 0);
	gtk_box_pack_start (box, check, FALSE, FALSE, 0);

	gtk_box_pack_start (lbox, lentry, TRUE, TRUE, 0);
	gtk_box_pack_start (lbox, lbrowse, FALSE, FALSE, 0);

	gtk_box_pack_start (rbox, rentry, TRUE, TRUE, 0);
	gtk_box_pack_start (rbox, rbrowse, FALSE, FALSE, 0);

	gnome_dialog_set_parent (GNOME_DIALOG (dialog), parent);
	gtk_window_set_default_size (GTK_WINDOW (dialog), 100, 100);

	gtk_widget_grab_focus (lentry);
	gtk_widget_show_all (dialog);

	return dialog;
}

