/* $Revision: 1.10 $ */

#include "options.h"

//XXX still missing tab size, exclusions list and extra options for diff
//XXX will need to be done manually

/*
this will be an object representing a diff's options
there will also an 'option' help by the global object (the defaults)

options_new will read from the config file
commit will save to cfg file

who will own the global options? refcount?
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

void
options_save (Options *options)
{
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

/*________________________________________________________________________________
*/

// If I add a frame name and a long text, then I can probably automate the whole lot!

PrefOption options[] =
{
	{ PrefPage,	N_("Style") },
	{ PrefFrame,	N_("Directory") },
	{ PrefStyle,	N_("Same"),					"DirStyleSame",			G_STRUCT_OFFSET (Options, DirStyleSame) },
	{ PrefStyle,	N_("Left"),					"DirStyleLeft",			G_STRUCT_OFFSET (Options, DirStyleLeft) },
	{ PrefStyle,	N_("Right"),					"DirStyleRight",		G_STRUCT_OFFSET (Options, DirStyleRight) },
	{ PrefStyle,	N_("Diff"),					"DirStyleDiff",			G_STRUCT_OFFSET (Options, DirStyleDiff) },
	{ PrefStyle,	N_("Error / Type"),				"DirStyleError",		G_STRUCT_OFFSET (Options, DirStyleError) },
	{ PrefFrame,	N_("File") },
	{ PrefStyle,	N_("Left"),					"FileStyleLeft",		G_STRUCT_OFFSET (Options, FileStyleLeft) },
	{ PrefStyle,	N_("Right"),					"FileStyleRight",		G_STRUCT_OFFSET (Options, FileStyleRight) },

	{ PrefPage,	N_("Confirmation") },
	{ PrefFrame,	N_("Confirmation") },
	{ PrefCheck,	N_("Confirm on close view"),			"ConfirmOnClose",		G_STRUCT_OFFSET (Options, ConfirmOnClose) },
	{ PrefCheck,	N_("Confirm on exit"),				"ConfirmOnExit",		G_STRUCT_OFFSET (Options, ConfirmOnExit) },
	{ PrefCheck,	N_("Close directory implies close all files"),	"ConfirmDirMeansAllFiles",	G_STRUCT_OFFSET (Options, ConfirmDirMeansAllFiles) },
	{ PrefCheck,	N_("Automatically browse on open"),		"AutoBrowseOnOpen",		G_STRUCT_OFFSET (Options, AutoBrowseOnOpen) },

	{ PrefPage,	N_("View") },
	{ PrefFrame,	N_("View") },
	{ PrefCheck,	N_("Same"),					"DirViewSame",			G_STRUCT_OFFSET (Options, DirViewSame) },
	{ PrefCheck,	N_("Left"),					"DirViewLeft",			G_STRUCT_OFFSET (Options, DirViewLeft) },
	{ PrefCheck,	N_("Right"),					"DirViewRight",			G_STRUCT_OFFSET (Options, DirViewRight) },
	{ PrefCheck,	N_("Diff"),					"DirViewDiff",			G_STRUCT_OFFSET (Options, DirViewDiff) },
	{ PrefCheck,	N_("Error / Type"),				"DirViewError",			G_STRUCT_OFFSET (Options, DirViewError) },

	{ PrefPage,	N_("Directory") },
	{ PrefFrame,	N_("Display style") },
	{ PrefRadio,	N_("Tree"),					"DirViewStyle",			G_STRUCT_OFFSET (Options, DirViewStyle) },				// Tree,				list (path),				list (no path)
	{ PrefRadio,	N_("List of files (full path)") },
	{ PrefRadio,	N_("List of files (no path)") },
	{ PrefFrame,	N_("Directory options") },
	{ PrefCheck,	N_("Statistics (very slow)"),			"DirStatistics",		G_STRUCT_OFFSET (Options, DirStatistics) },
	{ PrefCheck,	N_("File count"),				"DirFileCount",			G_STRUCT_OFFSET (Options, DirFileCount) },

	{ PrefPage,	N_("File") },
	{ PrefFrame,	N_("Pane style") },
	{ PrefRadio,	N_("One pane"),					"FilePaneStyle",		G_STRUCT_OFFSET (Options, FilePaneStyle) },
	{ PrefRadio,	N_("Two panes") },
	{ PrefFrame,	N_("File options") },
	{ PrefCheck,	N_("Scrollbar"),				"FileScrollbar",		G_STRUCT_OFFSET (Options, FileScrollbar) },
	{ PrefCheck,	N_("Overview"),					"FileOverview",			G_STRUCT_OFFSET (Options, FileOverview) },
	{ PrefCheck,	N_("Line numbers"),				"FileLineNumbers",		G_STRUCT_OFFSET (Options, FileLineNumbers) },

	{ PrefPage,	N_("Diff") },
	{ PrefFrame,	N_("Diff options") },
	{ PrefCheck,	N_("Ignore case"),				"DiffIgnoreCase",		G_STRUCT_OFFSET (Options, DiffIgnoreCase) },
	{ PrefCheck,	N_("Ignore all space"),				"DiffIgnoreAllSpace",		G_STRUCT_OFFSET (Options, DiffIgnoreAllSpace) },
	{ PrefCheck,	N_("Ignore space change"),			"DiffIgnoreSpaceChange",	G_STRUCT_OFFSET (Options, DiffIgnoreSpaceChange) },
	{ PrefCheck,	N_("Ignore blank lines"),			"DiffIgnoreBlankLines",		G_STRUCT_OFFSET (Options, DiffIgnoreBlankLines) },
	{ PrefCheck,	N_("Minimal"),					"DiffMinimal",			G_STRUCT_OFFSET (Options, DiffMinimal) },
	{ PrefCheck,	N_("Small changes in a large file"),		"DiffSmallChanges",		G_STRUCT_OFFSET (Options, DiffSmallChanges) },
	{ PrefCheck,	N_("Treat as text"),				"DiffTreatAsText",		G_STRUCT_OFFSET (Options, DiffTreatAsText) },

	{ PrefPage,	N_("MDI") },
	{ PrefFrame,	N_("MDI style") },
	{ PrefRadio,	N_("Default"),					"MDIStyle",			G_STRUCT_OFFSET (Options, MDIStyle) },				// Default,				Top Level,				Modal,				Notebook
	{ PrefRadio,	N_("Top Level") },
	{ PrefRadio,	N_("Modal") },
	{ PrefRadio,	N_("Notebook") },
	{ PrefFrame,	N_("Session Management") },
	{ PrefRadio,	N_("Open nothing"),				"SessionStyle",			G_STRUCT_OFFSET (Options, SessionStyle) },				// Open nothing,				Open,				but require rescan,				Rescan everything
	{ PrefRadio,	N_("Open, but require rescan") },
	{ PrefRadio,	N_("Rescan everything") },

	{ PrefPage,	N_("Exclusions") },
	{ PrefFrame,	N_("Exclude files matching") },
	{ PrefList,	NULL,						"Exclusions",			G_STRUCT_OFFSET (Options, Exclusions) },

	{ 0 },
 };

/*________________________________________________________________________________
*/

static GtkContainer *
add_page (GnomePropertyBox *props, PrefOption *opt)
{
	GtkWidget *label = gtk_label_new (_(opt->label));
	GtkWidget *vbox  = gtk_vbox_new  (FALSE, GNOME_PAD);

	gnome_property_box_append_page (props, vbox, label);

	return GTK_CONTAINER (vbox);
}

static GtkContainer *
add_frame (GtkContainer *box, PrefOption *opt)
{
	GtkWidget *frame = gtk_frame_new (_(opt->label));
	GtkWidget *vbox  = gtk_vbox_new  (FALSE, GNOME_PAD_SMALL);

	gtk_box_pack_start (GTK_BOX (box), frame, FALSE, FALSE, GNOME_PAD_SMALL);

	gtk_container_add (GTK_CONTAINER (frame), vbox);

	return GTK_CONTAINER (vbox);
}

static int
add_check (GtkContainer *cont, PrefOption *opt)
{
	gtk_container_add (cont, gtk_check_button_new_with_label (_(opt->label)));
	return 1;
}

static int
add_label (GtkContainer *cont, PrefOption *opt)
{
	gtk_container_add (cont, gtk_label_new (opt->label));
	return 1;
}

static int
add_radio (GtkContainer *cont, PrefOption *opt)
{
	GtkRadioButton *radio = NULL;
	int		count = 0;

	for (; opt->type == PrefRadio; opt++, count++)
	{
		radio = GTK_RADIO_BUTTON (gtk_radio_button_new_with_label_from_widget (radio, opt->label));

		gtk_container_add (cont, GTK_WIDGET (radio));
	}
	
	return count;
}

static int
add_style (GtkContainer *cont, PrefOption *opt)
{
	gtk_container_add (cont, gtk_check_button_new_with_label (_(opt->label)));
	return 1;
}

static int
add_list (GtkContainer *cont, PrefOption *opt)
{
	return 1;
}

GtkWidget *
get_preferences (GtkWindow *parent, PrefsPage page)
{
	GnomePropertyBox *props = NULL;
	PrefOption       *opt   = NULL;
	GtkContainer     *cont  = NULL;

	props = GNOME_PROPERTY_BOX (gnome_property_box_new());

	for (opt = options; opt->type;)
	{
		switch (opt->type)
		{
		case PrefPage:	cont = add_page  (props, opt);
				opt++;
				break;

		case PrefFrame:	cont = add_frame (cont, opt);
				opt++;
				break;

		case PrefCheck:	opt += add_check (cont, opt);
				break;

		case PrefLabel:	opt += add_label (cont, opt);
				break;

		case PrefRadio:	opt += add_radio (cont, opt);
				break;

		case PrefStyle:	opt += add_style (cont, opt);
				break;

		case PrefList:	opt += add_list  (cont, opt);
				break;

		default:	g_assert_not_reached();
				opt++;
				break;
		}
	}

	//set_default_page (page);
	//gtk_window_set_default_size (GTK_WINDOW (props), 400, 400);
	gtk_window_set_transient_for (GTK_WINDOW (props), parent);

	return GTK_WIDGET (props);
}

