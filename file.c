/* $Revision: 1.10 $ */

#include <gnome.h>
#include "file.h"
#include "options.h"
#include "diff.h"
#include "mdi.h"
#include "args.h"

/*
typedef struct _Reply Reply;

struct _Reply
{
	guint button;
};
*/

void
cmw_destroy_cb (GtkWidget * widget)
{
	g_print ("destroyed\n");
	//gtk_main_quit (); /* This is needed to get out of gtk_main */
}

void
file_selection_ok (GtkWidget * w, GtkFileSelection * dialog)
{
	//g_print ("%s\n", gtk_file_selection_get_filename (dialog));
	gtk_main_quit();
}

void
file_selection_cancel (GtkWidget * w, GtkFileSelection * dialog)
{
	//gtk_file_selection_set_filename (dialog, "");
	gtk_entry_set_text (GTK_ENTRY (dialog->selection_entry), "");
	gtk_main_quit();
}

char *
get_filename (GtkFileSelection *dialog)
{
	GtkEntry *entry = GTK_ENTRY (dialog->selection_entry);

	if (strlen (gtk_entry_get_text (entry)) == 0)
	{
		return NULL;
	}
	else
	{
		return strdup (gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog)));
	}
}

//static gboolean
//gnome_dialog_quit_run(GnomeDialog *dialog,
		      //struct GnomeDialogRunInfo *runinfo)

static void
dialog_button_destroyed (GtkWidget *widget, gpointer data)
{
	g_print ("dialog_button_destroyed\n");
}

static GnomeMDI *global_mdi = NULL;

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
			DiffOptions *diff = diffoptions_new();

			diff->left = strdup ("a/Makefile");
			diff->right = strdup ("b/Makefile");
			categorise_args (diff);
			//g_print ("type  = %s\n", (diff->type == File) ? "file" : "dir");

			mdi_add_diff (global_mdi, diff);
			gnome_dialog_close (dialog);				// OK
			break;
		}
		case 2: gnome_dialog_close (dialog);				// Cancel
			break;

		default: g_assert_not_reached();
			break;
	}
}

#if 0
void
new_file (GnomeMDI *mdi, GtkWindow *parent)
{
	static GtkWidget *dialog = NULL;

	g_print ("new_diff_cb\n");

	if (dialog)
	{
		g_assert(GTK_WIDGET_REALIZED(dialog));
		gdk_window_show(dialog->window);
		gdk_window_raise(dialog->window);
	}
	else
	{
		dialog = gtk_file_selection_new (NULL);
		gtk_window_set_title (GTK_WINDOW (dialog), "Hello world");

		// this connects US to another window.  Most likely we'll die first.
		// if the parent dies, we'll die -- XXX NOT TRUE
		//gtk_window_set_transient_for (GTK_WINDOW (fs), GTK_WINDOW (parent));  get via the mdi (data)

		gtk_signal_connect (GTK_OBJECT (dialog), "destroy",
				    GTK_SIGNAL_FUNC (gtk_widget_destroyed), &dialog);

		gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button),
				    "clicked", GTK_SIGNAL_FUNC (file_selection_ok), dialog);
		gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->cancel_button),
				   "clicked", GTK_SIGNAL_FUNC (file_selection_cancel), dialog);//XXX
		//gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->cancel_button),
		//		   "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT (dialog));

		gtk_widget_show_all (dialog);
		gtk_main();

		if (dialog)
		{
			//window = gtk_font_selection_dialog_new ("Font Selection Dialog");
			//window = gtk_color_selection_dialog_new ("color selection dialog");
			g_print ("filename '%s'\n", get_filename (GTK_FILE_SELECTION (dialog)));
			gtk_widget_destroy (GTK_WIDGET (dialog));
		}
		else
		{
			g_print ("no dialog\n");
			gtk_main_quit();
		}
	}
}
#endif

GtkWidget *
new_file2 (GnomeMDI *mdi, GtkWindow *parent)
{
	GtkWidget *dialog = NULL;
	//static GtkWidget *left   = NULL;
	//static GtkWidget *right  = NULL;

	//GtkWidget *table = NULL;
	GtkBox *box = NULL;
	GtkBox *lbox=NULL;
	GtkBox *rbox= NULL;
	GtkWidget *lentry = NULL;
	GtkWidget *rentry = NULL;
	GtkWidget *lbrowse = NULL;
	GtkWidget *rbrowse = NULL;
	GtkWidget *check   = NULL;
	//int i = 0;

	global_mdi = mdi;
	dialog = gnome_dialog_new ("Select files or directories to be compared",
		"Diff options",
		GNOME_STOCK_BUTTON_OK,
		GNOME_STOCK_BUTTON_CANCEL,
		NULL);

	// if we're closed then the dialog NULLs its parent (us) pointer
	gtk_window_set_transient_for (GTK_WINDOW (dialog), parent);

	gtk_signal_connect (GTK_OBJECT (dialog), "clicked", dialog_button_clicked, NULL);
	gtk_signal_connect (GTK_OBJECT (dialog), "destroy", dialog_button_destroyed, NULL);

	box = GTK_BOX ((GNOME_DIALOG (dialog))->vbox);

	lentry = gtk_entry_new();
	rentry = gtk_entry_new();

	lbox = GTK_BOX (gtk_hbox_new (FALSE, GNOME_PAD_SMALL));
	rbox = GTK_BOX (gtk_hbox_new (FALSE, GNOME_PAD_SMALL));

	lbrowse = gtk_button_new_with_label ("Browse...");
	rbrowse = gtk_button_new_with_label ("Browse...");

	check = gtk_check_button_new_with_label ("Start browse automatically");

	gtk_box_pack_start (box, GTK_WIDGET(lbox), FALSE, FALSE, 0);
	gtk_box_pack_start (box, GTK_WIDGET(rbox), FALSE, FALSE, 0);
	gtk_box_pack_start (box, check, FALSE, FALSE, 0);

	gtk_box_pack_start (lbox, lentry, TRUE, TRUE, 0);
	gtk_box_pack_start (lbox, lbrowse, FALSE, FALSE, 0);

	gtk_box_pack_start (rbox, rentry, TRUE, TRUE, 0);
	gtk_box_pack_start (rbox, rbrowse, FALSE, FALSE, 0);

	gnome_dialog_set_parent (GNOME_DIALOG (dialog), parent);
	gtk_window_set_default_size (GTK_WINDOW (dialog), 100, 100);

	gtk_widget_show_all (dialog);
	//gnome_dialog_run (GNOME_DIALOG (dialog));

	return dialog;
}

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

#if 0
	//XXX neat!
	gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "focus_in_event",
						(GtkSignalFunc) gtk_widget_grab_default,
						GTK_OBJECT (filesel->ok_button));
	gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "activate",
						(GtkSignalFunc) gtk_button_clicked,
						GTK_OBJECT (filesel->ok_button));
#endif

