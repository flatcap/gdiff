/* $Revision: 1.8 $ */

#include <gnome.h>
#include "file.h"

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
void
new_file (GnomeMDI *mdi, GtkWindow *parent)
{
	static GtkWidget *dialog = NULL;
	//static GtkWidget *left   = NULL;
	//static GtkWidget *right  = NULL;

	//GtkWidget *table = NULL;
	GtkBox *box = NULL;
	//int i = 0;

	dialog = gnome_dialog_new ("Select files or directories to be compared", GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);

	box = GTK_BOX ((GNOME_DIALOG (dialog))->vbox);

	gtk_box_pack_start (box, gtk_entry_new(), TRUE, TRUE, 0);
	gtk_box_pack_start (box, gtk_entry_new(), TRUE, TRUE, 0);

	gnome_dialog_set_parent (GNOME_DIALOG (dialog), parent);
	gtk_window_set_default_size (GTK_WINDOW (dialog), 100, 100);

	gtk_widget_show_all (dialog);
	gnome_dialog_run (GNOME_DIALOG (dialog));
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

#if 0
		if (!GTK_WIDGET_VISIBLE (window))
			gtk_widget_show_all (window);
		else
			gtk_widget_destroy (window);
#endif
	}
	else
	{
		dialog = gtk_file_selection_new (NULL);
		gtk_window_set_title (GTK_WINDOW (dialog), "Hello world");

		// this connects US to another window.  Most likely we'll die first.
		// if the parent dies, we'll die
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

	//XXX neat!
	//gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "focus_in_event",
						//(GtkSignalFunc) gtk_widget_grab_default,
						//GTK_OBJECT (filesel->ok_button));
	//gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "activate",
						//(GtkSignalFunc) gtk_button_clicked,
						//GTK_OBJECT (filesel->ok_button));
}
#endif

