/* $Revision: 1.8 $ */

#include "options.h"

/*
this will be an object representing a diff's options
there will also an 'option' help by the global object (the defaults)

options_new will read from the config file
commit will save to cfg file

who will own the global options? refcount?
*/

/*
Style	F tree	 	S same
			S diff
			S left
			S right
			S error/type
	F compare	S left
			S right
Confirmation
	Are you sure?
		close view
		exit
	close tree -> close all files
	automatically open browse on file/open
View		X same
		X left
		X right
		X diff
		X error/type
Tree (dirs)
	O display as a tree
	O display as list of files
		O with path
		O no path
	X statistics (very slow)
	X file count
Compare
	O One pane
	O Two panes
	X Scrollbar
	X Overview
	X Line numbers
	tab width [spinner]
Diff options
	X Ignore case
	X ignore all space
	X ignore space change
	X ignore blank lines
	X minimal
	X small changes in a large file
	X treat as text
	extra options [      ] at own risk!
MDI	MDI Style		O default
				O top level
				O modal
				O notebook
	Session management	O open nothing
				O open but require rescan
				O rescan everything
Exclusions
	L Exclude files
*/

/*----------------------------------------------------------------------------*/
//static Options * options_new (void);
//static gboolean options_save_options (Options *options);
//static gboolean options_set_options  (Options *options);
//static void options_free (Options *options);

Options * options_get_default (void);
/*----------------------------------------------------------------------------*/

Options *
options_get_default (void)
{
	// ask global for this first
	// read from the config file
	return g_malloc0 (sizeof (Options));
}

/*
static Options *
options_new (void)
{
	return NULL;
}
*/

/*
static void
options_free (Options *options)
{
	g_free (options);
}
*/

/*
static gboolean
options_set_options  (Options *options)
{
	return FALSE;
}
*/

/*
static gboolean
options_save_options (Options *options)
{
	gboolean result = FALSE;

	if (options_set_options (options))
	{
		//save to config file
	}

	return result;
}
*/

/* F Frame S Style O Radio X Check L List */
/* frame_cb, style_cb, radio_cb, check_cb, list_cb */

static GtkBox *
preferences_create_base (GnomePropertyBox *props, char *tab)
{
	GtkWidget *label = gtk_label_new (tab);
	GtkWidget *vbox  = gtk_vbox_new  (FALSE, GNOME_PAD);

	gnome_property_box_append_page (props, vbox, label);

	return GTK_BOX (vbox);
}

static GtkBox *
preferences_create_frame (GtkBox *box, char *name)
{
	GtkWidget *frame = gtk_frame_new (name);
	GtkWidget *vbox  = gtk_vbox_new  (FALSE, GNOME_PAD_SMALL);

	gtk_box_pack_start (box, frame, FALSE, FALSE, GNOME_PAD_SMALL);

	gtk_container_add (GTK_CONTAINER (frame), vbox);

	return GTK_BOX (vbox);
}

static void
preferences_create_check_boxes (GtkContainer *cont, ...)	// NULL terminated
{
	char    *name = NULL;
	va_list  args;

	va_start (args, cont);
	
	while ((name = va_arg (args, char *)))
	{
		gtk_container_add (cont,  gtk_check_button_new_with_label (name));
	}
	
	va_end (args);
}

static void
preferences_create_radio_group (GtkContainer *cont, ...)	// NULL terminated
{
	char      *name  = NULL;
	GtkWidget *radio = NULL;
	va_list    args;

	va_start (args, cont);
	
	while ((name = va_arg (args, char *)))
	{
		radio = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio), name);

		gtk_container_add (cont, radio);
	}
	
	va_end (args);
}

static void
preferences_add_style (GnomePropertyBox *props)
{
	GtkBox *base = NULL;
	GtkBox *dir  = NULL;
	GtkBox *file = NULL;

	base = preferences_create_base  (props, "Style");
	dir  = preferences_create_frame (base,  "Directory");
	file = preferences_create_frame (base,  "File");

	preferences_create_check_boxes (GTK_CONTAINER (dir),  "Same", "Left", "Right", "Diff", "Error / Type", NULL);
	preferences_create_check_boxes (GTK_CONTAINER (file), "Left", "Right", NULL);
}

static void
preferences_add_confirmation (GnomePropertyBox *props)
{
	GtkBox *base = preferences_create_base  (props, "Confirmation");
	GtkBox *conf = preferences_create_frame (base,  "Confirmation");

	preferences_create_check_boxes (GTK_CONTAINER (conf), "Confirm on close view", "Confirm on exit", "Close directory implies close all files", "Automatically browse on open", NULL);
}

static void
preferences_add_view (GnomePropertyBox *props)
{
	GtkBox *base = preferences_create_base  (props, "View");
	GtkBox *view = preferences_create_frame (base,  "View");

	preferences_create_check_boxes (GTK_CONTAINER (view), "Same", "Left", "Right", "Diff", "Error / Type",  NULL);
}

static void
preferences_add_directory (GnomePropertyBox *props)
{
	GtkBox *base = preferences_create_base  (props, "Directory");
	GtkBox *disp = preferences_create_frame (base,  "Display style");
	GtkBox *opts = preferences_create_frame (base,  "Directory options");

	preferences_create_radio_group (GTK_CONTAINER (disp), "Tree", "List of files (full path)", "List of files (no path)", NULL);
	preferences_create_check_boxes (GTK_CONTAINER (opts), "Statistics (very slow)", "File count", NULL);
}

static void
preferences_add_file (GnomePropertyBox *props)
{
	GtkBox *base = preferences_create_base  (props, "File");
	GtkBox *pane = preferences_create_frame (base,  "Pane style");
	GtkBox *file = preferences_create_frame (base,  "File options");

	preferences_create_radio_group (GTK_CONTAINER (pane), "One pane", "Two panes", NULL);
	preferences_create_check_boxes (GTK_CONTAINER (file), "Scrollbar", "Overview", "Line numbers", NULL);
}

static void
preferences_add_diff (GnomePropertyBox *props)
{
	GtkBox *base = preferences_create_base  (props, "Diff");
	GtkBox *diff = preferences_create_frame (base,  "Diff options");

	preferences_create_check_boxes (GTK_CONTAINER (diff), "Ignore case", "Ignore all space", "Ignore space change", "Ignore blank lines", "Minimal", "Small changes in a large file", "Treat as text", NULL);
}

static void
preferences_add_mdi (GnomePropertyBox *props)
{
	GtkBox *base  = preferences_create_base  (props, "MDI");
	GtkBox *mdi   = preferences_create_frame (base,  "MDI style");
	GtkBox *sess  = preferences_create_frame (base,  "Session Management");

	preferences_create_radio_group (GTK_CONTAINER (mdi),  "Default", "Top Level", "Modal", "Notebook", NULL);
	preferences_create_radio_group (GTK_CONTAINER (sess), "Open nothing", "Open, but require rescan", "Rescan everything", NULL);
}

static void
preferences_add_exclusions (GnomePropertyBox *props)
{
	GtkWidget *label1 = gtk_label_new ("preferences_exclusions");
	GtkWidget *label2 = gtk_label_new ("exclusions");

	gnome_property_box_append_page (props, label1, label2);
}

GtkWidget *
get_preferences (GtkWindow *parent)
{
	GnomePropertyBox *props = NULL;

	props = GNOME_PROPERTY_BOX (gnome_property_box_new());

	//XXX Need a reset button

	preferences_add_style        (props);
	preferences_add_confirmation (props);
	preferences_add_view         (props);
	preferences_add_directory    (props);
	preferences_add_file         (props);
	preferences_add_diff         (props);
	preferences_add_mdi          (props);
	preferences_add_exclusions   (props);

	//gtk_window_set_default_size (GTK_WINDOW (props), 400, 400);

	return GTK_WIDGET (props);
}

