/* $Revision: 1.26 $ */

#include "config.h"
#include <gnome.h>
#include "menu.h"
#include "derived.h"
#include "node.h"
#include "tree.h"
#include "compare.h"
#include "mdi.h"

static void about_cb		(GtkWidget *widget, gpointer data) { g_print ("about_cb\n"); }
static void close_view_cb	(GtkWidget *widget, gpointer data) { g_print ("close_view_cb\n"); }
static void compare_cb		(GtkWidget *widget, gpointer data) { g_print ("compare_cb\n"); }
static void contents_cb		(GtkWidget *widget, gpointer data) { g_print ("contents_cb\n"); }
static void exit_gdiff_cb	(GtkWidget *widget, gpointer data) { g_print ("exit_gdiff_cb\n"); }
static void next_diff_cb	(GtkWidget *widget, gpointer data) { g_print ("next_diff_cb\n"); }
static void preferences_cb	(GtkWidget *widget, gpointer data) { g_print ("preferences_cb\n"); }
static void prev_diff_cb	(GtkWidget *widget, gpointer data) { g_print ("prev_diff_cb\n"); }
static void rescan_cb		(GtkWidget *widget, gpointer data) { g_print ("rescan_cb\n"); }
static void save_file_list_cb	(GtkWidget *widget, gpointer data) { g_print ("save_file_list_cb\n"); }
static void status_bar_cb	(GtkWidget *widget, gpointer data) { g_print ("status_bar_cb\n"); }
static void view_cb		(GtkWidget *widget, gpointer data) { g_print ("view_cb\n"); }

typedef struct _Reply Reply;

struct _Reply
{
	guint button;
};

static void
cmw_destroy_cb (GtkWidget * widget)
{
	g_print ("destroyed\n");
	//gtk_main_quit (); /* This is needed to get out of gtk_main */
}

void
file_selection_ok (GtkWidget * w,
		   GtkFileSelection * dialog)
{
	//g_print ("%s\n", gtk_file_selection_get_filename (dialog));
	gtk_main_quit();
}

void
file_selection_cancel (GtkWidget * w,
		   GtkFileSelection * dialog)
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

static void new_diff_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog = NULL;

	g_print ("new_diff_cb\n");

	dialog = gtk_file_selection_new ("This is a modal file selection dialog");
	//!!! gtk_window_set_title (GTK_WINDOW (filesel), title);
	//gtk_window_set_transient_for (GTK_WINDOW (fs), GTK_WINDOW (parent));  get via the mdi (data)
	gtk_signal_connect (GTK_OBJECT (dialog), "destroy",
			    GTK_SIGNAL_FUNC (cmw_destroy_cb), NULL);

	gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button),
			    "clicked", GTK_SIGNAL_FUNC (file_selection_ok), dialog);
	gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->cancel_button),
			   "clicked", GTK_SIGNAL_FUNC (file_selection_cancel), dialog);//XXX
	//gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->cancel_button),
	//		   "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT (dialog));

	gtk_widget_show_all (dialog);
	gtk_main();
	//window = gtk_font_selection_dialog_new ("Font Selection Dialog");
	//window = gtk_color_selection_dialog_new ("color selection dialog");
	g_print ("filename '%s'\n", get_filename (GTK_FILE_SELECTION (dialog)));
	gtk_widget_destroy (GTK_WIDGET (dialog));

  //XXX neat!
  //gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "focus_in_event",
			     //(GtkSignalFunc) gtk_widget_grab_default,
			     //GTK_OBJECT (filesel->ok_button));
  //gtk_signal_connect_object (GTK_OBJECT (filesel->selection_entry), "activate",
                             //(GtkSignalFunc) gtk_button_clicked,
                             //GTK_OBJECT (filesel->ok_button));
}

// XXX method to automate menu creation, callbacks, signals and accelerators!

/*----------------------------------------------------------------------------*/
//static DiffOptions * get_current_selection (GnomeMDI *mdi);
//static GtkWidget * get_current_view (GnomeMDI *mdi);
//static void menu_set_view_defaults (GtkMenuShell *shell);
//static void about_orig_cb		(GtkWidget * widget, gpointer data);
//static void file_close_cb	(GtkWidget * widget, gpointer data);
//static void file_open_cb	(GtkWidget * widget, gpointer data);
//static void show_cb		(GtkWidget * widget, gpointer data);
//static void view_orig_cb		(GtkWidget * widget, gpointer data);

void menu_create (GnomeMDI *mdi, GnomeApp *app);
/*----------------------------------------------------------------------------*/

GtkStatusbar *global_statusbar = NULL;

#define GNOMEUIINFO_MENU_DIFF_TREE(tree) { GNOME_APP_UI_SUBTREE_STOCK, N_("_Diff"), NULL, tree, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL }

//#define GNOMEUIINFO_ITEM            (label, tooltip, callback, xpm_data)
//#define GNOMEUIINFO_ITEM_STOCK      (label, tooltip, callback, stock_id)
//#define GNOMEUIINFO_ITEM_NONE       (label, tooltip, callback)
//#define GNOMEUIINFO_ITEM_DATA       (label, tooltip, callback, user_data, xpm_data)
//#define GNOMEUIINFO_TOGGLEITEM      (label, tooltip, callback, xpm_data)
//#define GNOMEUIINFO_TOGGLEITEM_DATA (label, tooltip, callback, user_data, xpm_data)
//#define GNOMEUIINFO_SEPARATOR

///XXX <libgnome/gnome-uidefs.h>

// need pointers to settings menu s/l/r/d, status bar
// change toggle to checkbox X Show status bar

#if 0
typedef struct {
	GnomeUIInfoType type;
	gchar *label;
	gchar *hint;
	gpointer moreinfo;
	gpointer user_data;
	gpointer unused_data;
	GnomeUIPixmapType pixmap_type;
	gpointer pixmap_info;
	guint accelerator_key;
	GdkModifierType ac_mods;
	GtkWidget *widget;
} GnomeUIInfo;
#endif

static GnomeUIInfo file_menu[] =
{
	GNOMEUIINFO_MENU_SAVE_AS_ITEM(save_file_list_cb, NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM(close_view_cb, NULL),
	GNOMEUIINFO_MENU_EXIT_ITEM(exit_gdiff_cb, NULL),
	GNOMEUIINFO_END
};

static GnomeUIInfo diff_menu[] =
{
	{ GNOME_APP_UI_ITEM, "_New Difference", "", new_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_n, GDK_CONTROL_MASK, NULL },

	GNOMEUIINFO_SEPARATOR,

	{ GNOME_APP_UI_ITEM, "_Previous Difference", "", prev_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F9, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "_Next Difference",     "", next_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F10, (GdkModifierType) 0, NULL },
	
	GNOMEUIINFO_SEPARATOR,

	{ GNOME_APP_UI_ITEM, "_Compare", "", compare_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_c, GDK_CONTROL_MASK, NULL },
	{ GNOME_APP_UI_ITEM, "_Rescan",  "", rescan_cb,  NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_r, GDK_CONTROL_MASK, NULL },

	GNOMEUIINFO_END
};

static GnomeUIInfo view_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "_Same", "Same tooltip", view_cb, GUINT_TO_POINTER (eFileSame), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F5, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Left", "Left tooltip", view_cb, GUINT_TO_POINTER (eFileLeft), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F6, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Right", "Right tooltip", view_cb, GUINT_TO_POINTER (eFileRight), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F7, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Different", "Different tooltip", view_cb, GUINT_TO_POINTER (eFileDiff), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F8, (GdkModifierType) 0, NULL },

	GNOMEUIINFO_END
};

static GnomeUIInfo settings[] =
{
	GNOMEUIINFO_TOGGLEITEM_DATA ("Show Status _Bar", NULL, status_bar_cb, NULL, NULL),
	//togg = gtk_menu_item_new_with_label ("Toggle Status _Bar");		// ctrl-b
	//GNOMEUIINFO_ITEM_NONE ("_Preferences", NULL, NULL),
	//pref = gtk_menu_item_new_with_label ("_Preferences");			// ctrl-p
	GNOMEUIINFO_MENU_PREFERENCES_ITEM (preferences_cb, NULL),
	GNOMEUIINFO_END
};

static GnomeUIInfo windows_menu[] =
{
	GNOMEUIINFO_END
};

static GnomeUIInfo help_menu[] =
{
	GNOMEUIINFO_ITEM_NONE ("_Contents", NULL, contents_cb),
	//cont  = gtk_menu_item_new_with_label ("_Contents");			// F1
	//GNOMEUIINFO_ITEM_NONE ("_About", NULL, NULL),
	GNOMEUIINFO_MENU_ABOUT_ITEM(about_cb, NULL),
	//about = gtk_menu_item_new_with_label ("_About");			// F2
	GNOMEUIINFO_END
};

static GnomeUIInfo main_menu[] =
{
	GNOMEUIINFO_MENU_FILE_TREE     (file_menu),
	GNOMEUIINFO_MENU_DIFF_TREE     (diff_menu),
	GNOMEUIINFO_MENU_VIEW_TREE     (view_menu),
	GNOMEUIINFO_MENU_SETTINGS_TREE (settings),
	GNOMEUIINFO_MENU_WINDOWS_TREE  (windows_menu),
	GNOMEUIINFO_MENU_HELP_TREE     (help_menu),
	GNOMEUIINFO_END
};

/*
static GnomeUIInfo file_menu[] =
{
	GNOMEUIINFO_MENU_OPEN_ITEM  (file_open_cb,  NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM (file_close_cb, NULL),
	GNOMEUIINFO_END
};

static GnomeUIInfo empty_menu[] =
{
	GNOMEUIINFO_END
};

static GnomeUIInfo view_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Identical"), N_("Show files that are the same"),           view_orig_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F5, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Left"),      N_("Show files that only appear in 'left'"),  view_orig_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F6, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Right"),     N_("Show files that only appear in 'right'"), view_orig_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F7, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Different"), N_("Show files that are different"),          view_orig_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F8, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo compare_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Show diff"), N_("asfasdfShow files that are the same"),     show_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F5, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo help_menu[] =
{
	GNOMEUIINFO_MENU_ABOUT_ITEM (about_orig_cb, NULL),
	GNOMEUIINFO_END
};

static GnomeUIInfo main_menu[] =
{
	GNOMEUIINFO_MENU_FILE_TREE (file_menu),
	GNOMEUIINFO_MENU_VIEW_TREE (view_menu),
	{ GNOME_APP_UI_SUBTREE, ("Children"), NULL, empty_menu,   NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, 0, 0, NULL },
	{ GNOME_APP_UI_SUBTREE, ("Compare"),  NULL, compare_menu, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, 0, 0, NULL },
	GNOMEUIINFO_MENU_HELP_TREE (help_menu),
	GNOMEUIINFO_END
};
*/

/*
static GnomeUIInfo dummy_menu[] =
{
	GNOMEUIINFO_END
};
*/
/*
static void
menu_set_view_defaults (GtkMenuShell *shell)
{
	GList            *list  = NULL;
	GtkLabel         *label = NULL;
	GtkBin           *bin   = NULL;
	GtkMenuShell     *sub   = NULL;
	GtkMenuItem      *item  = NULL;
	GtkCheckMenuItem *check = NULL;

	list = shell->children;
	while (list)
	{
		item  = GTK_MENU_ITEM (list->data);
		bin   = GTK_BIN       (item);
		label = GTK_LABEL     (bin->child);

		if (strcmp (label->label, "View") == 0)
		{
			sub = GTK_MENU_SHELL (item->submenu);
			break;
		}

		list = list->next;
	}

	if (sub)
	{
		list = sub->children;
		while (list)
		{
			if (GTK_IS_CHECK_MENU_ITEM (list->data))	// Skip the tear-off menu
			{
				check  = GTK_CHECK_MENU_ITEM (list->data);
				bin    = GTK_BIN             (check);
				label  = GTK_LABEL           (bin->child);

				check->active = TRUE;
			}

			list = list->next;
		}
	}
}
*/

GtkWidget * create_file_menu();
GtkWidget * create_diff_menu();
GtkWidget * create_view_menu();
GtkWidget * create_settings_menu();
GtkWidget * create_window_menu();
GtkWidget * create_help_menu();

void
menu_create (GnomeMDI *mdi, GnomeApp *app)
{
	GtkWidget *status = NULL;
	GnomeUIInfo *window = NULL;

	//GtkMenuBar *menubar = NULL;

	gnome_app_create_menus_with_data (app, main_menu, NULL);

	status = gtk_statusbar_new();
	//global_statusbar = GTK_STATUSBAR (status);

	gnome_app_install_statusbar_menu_hints (GTK_STATUSBAR (status), main_menu);
	gnome_app_set_statusbar (GNOME_APP (app), status);

	window = &main_menu[4];

	/*
	g_print ("type      %d\n", window->type);
	g_print ("label     %s\n", window->label);
	g_print ("hint      %s\n", window->hint);
	g_print ("submenu   %p\n", window->moreinfo);
	g_print ("user      %p\n", window->user_data);
	g_print ("unused    %p\n", window->unused_data);
	g_print ("pixtype   %d\n", window->pixmap_type);
	g_print ("pixinfo   %p\n", window->pixmap_info);
	g_print ("accel     %d\n", window->accelerator_key);
	g_print ("acmods    %d\n", window->ac_mods);
	g_print ("widget    %s\n", gtk_widget_get_name (window->widget));
	*/

	gtk_widget_set_sensitive (window->widget, FALSE);

	/*
	menubar = GTK_MENU_BAR (app->menubar);

	gtk_menu_bar_append (menubar, create_file_menu());
	gtk_menu_bar_append (menubar, create_diff_menu());
	gtk_menu_bar_append (menubar, create_view_menu());
	gtk_menu_bar_append (menubar, create_settings_menu());
	gtk_menu_bar_append (menubar, create_window_menu());
	gtk_menu_bar_append (menubar, create_help_menu());

	gtk_widget_show_all (app->menubar);
	*/
	/*
	GtkWidget *status = NULL;

	g_return_if_fail (app != NULL);

	gnome_app_create_menus_with_data (app, main_menu, mdi);

	g_return_if_fail (app->menubar != NULL);

	menu_set_view_defaults (GTK_MENU_SHELL (app->menubar));

	status = gtk_statusbar_new();
	global_statusbar = GTK_STATUSBAR (status);

	gnome_app_install_statusbar_menu_hints (GTK_STATUSBAR (status), main_menu);
	gnome_app_set_statusbar (GNOME_APP (app), status);
	*/

/*
	if these menus are created with data,
	then it must be a pointer to a tree
	will this ever happen?:

	data  -> tree
	!data -> MDI -> all trees?

	should view ident/diff/left/right be global or per tree?
*/
}

/*
static void
file_open_cb (GtkWidget * widget, gpointer data)
{
	//main_diff();
}

static void
file_close_cb (GtkWidget * widget, gpointer data)
{
	g_print ("file_close_cb\n");
}

static void
view_orig_cb (GtkWidget * widget, gpointer data)
{
	g_print ("view_orig_cb\n");
}

static void
about_orig_cb (GtkWidget * widget, gpointer data)
{
	// should only allow one of these, gdk_widget_raise it if called again
	// mustn't be modal
	GtkWidget      *about;
	const char     *authors[] = {"Richard Russon <Richard.Russon@ait.co.uk>", NULL};
	const char     *copyright = "Copyright 1999";
	const char     *extra = "Extra information";

	about = gnome_about_new (PACKAGE, VERSION, copyright, authors, extra, NULL);

	//gtk_window_set_transient_for (GTK_WINDOW (about), GTK_WINDOW (app));

	gtk_widget_show (about);

	return;
}
*/

/*
static GtkWidget *
get_current_view (GnomeMDI *mdi)
{
	GtkWidget *widget = NULL;
	GtkBin    *bin    = NULL;

	g_return_val_if_fail (mdi != NULL, NULL);
	widget = gnome_mdi_get_active_view (mdi);

	g_return_val_if_fail (widget != NULL, NULL);
	bin = GTK_BIN (widget);

	g_return_val_if_fail (bin != NULL, NULL);
	return bin->child;
}

static DiffOptions *
get_current_selection (GnomeMDI *mdi)
{
	GtkWidget	*view		= NULL;
	GtkDiffTree	*tree		= NULL;
	GtkCList	*clist		= NULL;
	GList		*selection	= NULL;
	GList		*list		= NULL;
	DiffTreeRow	*treerow	= NULL;
	DiffOptions	*options	= NULL;

	view = get_current_view (mdi);

	g_return_val_if_fail (view != NULL, NULL);
	tree  = GTK_DIFF_TREE (view);
	clist = GTK_CLIST     (tree);

	g_return_val_if_fail (clist != NULL, NULL);
	selection = clist->selection;

	g_return_val_if_fail (selection != NULL, NULL);
	list    = (selection->data);

	g_return_val_if_fail (list != NULL, NULL);
	treerow = (DiffTreeRow*) list->data;

	g_return_val_if_fail (treerow != NULL, NULL);
	options = diffoptions_new();

	g_return_val_if_fail (options != NULL, NULL);
	options->left  = g_strconcat (tree->left,  G_DIR_SEPARATOR_S, treerow->path, NULL);
	options->right = g_strconcat (tree->right, G_DIR_SEPARATOR_S, treerow->path, NULL);
	options->dir   = FALSE;

	return options;
}
*/
/*
static void
show_cb (GtkWidget * widget, gpointer data)
{
	GnomeMDI    *mdi  = NULL;
	DiffOptions *diff = NULL;

	mdi = GNOME_MDI (data);
	g_return_if_fail (mdi != NULL);

	diff = get_current_selection (mdi);

	g_return_if_fail (diff != NULL);
	mdi_add_diff (mdi, diff);
}
*/

//______________________________________________________________________________
//

GtkWidget *
create_file_menu (void)
{
	GtkMenu   *menu = GTK_MENU (gtk_menu_new());
	GtkWidget *save = NULL;
	GtkWidget *close = NULL;
	GtkWidget *exit = NULL;
	GtkWidget *file = NULL;

	g_return_val_if_fail (menu != NULL, NULL);

	save  = gtk_menu_item_new_with_label ("_Save File List");		// ctrl-s
	close = gtk_menu_item_new_with_label ("Close _View");			// ctrl-w
	exit  = gtk_menu_item_new_with_label ("E_xit GDiff");			// ctrl-q
	file  = gtk_menu_item_new_with_label ("_File");

	g_return_val_if_fail (save && close && exit && file, NULL);

	gtk_signal_connect (GTK_OBJECT (save),  "activate", GTK_SIGNAL_FUNC (save_file_list_cb), GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (close), "activate", GTK_SIGNAL_FUNC (close_view_cb),     GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (exit),  "activate", GTK_SIGNAL_FUNC (exit_gdiff_cb),     GUINT_TO_POINTER (0x1234));

	gtk_menu_append (menu, save);
	gtk_menu_append (menu, close);
	gtk_menu_append (menu, exit);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file), GTK_WIDGET (menu));

	return file;
}

GtkWidget *
create_diff_menu (void)
{
	GtkMenu   *menu = GTK_MENU (gtk_menu_new());
	GtkWidget *newd = NULL;
	GtkWidget *comp = NULL;
	GtkWidget *scan = NULL;
	GtkWidget *diff = NULL;

	g_return_val_if_fail (menu != NULL, NULL);

	newd = gtk_menu_item_new_with_label ("_New Diff");			// ctrl-n
	comp = gtk_menu_item_new_with_label ("_Compare");			// ctrl-c
	scan = gtk_menu_item_new_with_label ("_Rescan");			// ctrl-r
	diff = gtk_menu_item_new_with_label ("_Diff");

	g_return_val_if_fail (newd && comp && scan && diff, NULL);

	gtk_signal_connect (GTK_OBJECT (newd), "activate", GTK_SIGNAL_FUNC (new_diff_cb), GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (comp), "activate", GTK_SIGNAL_FUNC (compare_cb),  GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (scan), "activate", GTK_SIGNAL_FUNC (rescan_cb),   GUINT_TO_POINTER (0x1234));

	gtk_menu_append (menu, newd);
	gtk_menu_append (menu, comp);
	gtk_menu_append (menu, scan);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (diff), GTK_WIDGET (menu));

	return diff;
}

GtkWidget *
create_view_menu (void)
{
	GtkMenu   *menu  = GTK_MENU (gtk_menu_new());
	GtkWidget *same  = NULL;
	GtkWidget *left  = NULL;
	GtkWidget *right = NULL;
	GtkWidget *diff  = NULL;
	GtkWidget *prev  = NULL;
	GtkWidget *next  = NULL;
	GtkWidget *view  = NULL;

	g_return_val_if_fail (menu != NULL, NULL);

	same  = gtk_menu_item_new_with_label ("_Same");				// F5
	left  = gtk_menu_item_new_with_label ("_Left");				// F6
	right = gtk_menu_item_new_with_label ("_Right");			// F7
	diff  = gtk_menu_item_new_with_label ("_Different");			// F8
	prev  = gtk_menu_item_new_with_label ("_Previous Difference");		// F9
	next  = gtk_menu_item_new_with_label ("_Next Difference");		// F10
	view  = gtk_menu_item_new_with_label ("_View");

	g_return_val_if_fail (same && left && right && diff && prev && next && view, NULL);

	gtk_signal_connect (GTK_OBJECT (same),  "activate", GTK_SIGNAL_FUNC (view_cb),      GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (left),  "activate", GTK_SIGNAL_FUNC (view_cb),      GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (right), "activate", GTK_SIGNAL_FUNC (view_cb),      GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (diff),  "activate", GTK_SIGNAL_FUNC (view_cb),      GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (prev),  "activate", GTK_SIGNAL_FUNC (prev_diff_cb), GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (next),  "activate", GTK_SIGNAL_FUNC (next_diff_cb), GUINT_TO_POINTER (0x1234));

	gtk_menu_append (menu, same);
	gtk_menu_append (menu, left);
	gtk_menu_append (menu, right);
	gtk_menu_append (menu, diff);
	gtk_menu_append (menu, prev);
	gtk_menu_append (menu, next);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (view), GTK_WIDGET (menu));

	return view;
}

GtkWidget *
create_settings_menu (void)
{
	GtkMenu   *menu = GTK_MENU (gtk_menu_new());
	GtkWidget *togg = NULL;
	GtkWidget *pref = NULL;
	GtkWidget *sets = NULL;

	g_return_val_if_fail (menu != NULL, NULL);

	togg = gtk_menu_item_new_with_label ("Toggle Status _Bar");		// ctrl-b
	pref = gtk_menu_item_new_with_label ("_Preferences");			// ctrl-p
	sets = gtk_menu_item_new_with_label ("_Settings");

	g_return_val_if_fail (togg && pref && sets, NULL);

	gtk_signal_connect (GTK_OBJECT (togg), "activate", GTK_SIGNAL_FUNC (status_bar_cb),  GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (pref), "activate", GTK_SIGNAL_FUNC (preferences_cb), GUINT_TO_POINTER (0x1234));

	gtk_menu_append (menu, togg);
	gtk_menu_append (menu, pref);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (sets), GTK_WIDGET (menu));

	return sets;
}

GtkWidget *
create_window_menu (void)
{
	GtkMenu   *menu = GTK_MENU (gtk_menu_new());
	GtkWidget *wind = NULL;

	g_return_val_if_fail (menu != NULL, NULL);

	wind = gtk_menu_item_new_with_label ("_Window");

	g_return_val_if_fail (wind, NULL);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (wind), GTK_WIDGET (menu));

	return wind;
}

GtkWidget *
create_help_menu (void)
{
	GtkMenu   *menu  = GTK_MENU (gtk_menu_new());
	GtkWidget *cont  = NULL;
	GtkWidget *about = NULL;
	GtkWidget *help  = NULL;

	g_return_val_if_fail (menu != NULL, NULL);

	cont  = gtk_menu_item_new_with_label ("_Contents");			// F1
	about = gtk_menu_item_new_with_label ("_About");			// F2
	help  = gtk_menu_item_new_with_label ("_Help");

	g_return_val_if_fail (cont && about && help, NULL);

	gtk_signal_connect (GTK_OBJECT (cont),  "activate", GTK_SIGNAL_FUNC (contents_cb), GUINT_TO_POINTER (0x1234));
	gtk_signal_connect (GTK_OBJECT (about), "activate", GTK_SIGNAL_FUNC (about_cb),    GUINT_TO_POINTER (0x1234));

	gtk_menu_append (menu, cont);
	gtk_menu_append (menu, about);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (help), GTK_WIDGET (menu));

	return help;
}
#if 0

#define GNOME_MENU_FILE_STRING D_("_File")
#define GNOME_MENU_FILE_PATH D_("_File/")
#define GNOME_MENU_EDIT_STRING D_("_Edit")
#define GNOME_MENU_EDIT_PATH D_("_Edit/")
#define GNOME_MENU_VIEW_STRING D_("_View")
#define GNOME_MENU_VIEW_PATH D_("_View/")
#define GNOME_MENU_SETTINGS_STRING D_("_Settings")
#define GNOME_MENU_SETTINGS_PATH D_("_Settings/")
#define GNOME_MENU_NEW_STRING D_("_New")
#define GNOME_MENU_NEW_PATH D_("_New/")
#define GNOME_MENU_FILES_STRING D_("Fi_les")
#define GNOME_MENU_FILES_PATH D_("Fi_les/")
#define GNOME_MENU_WINDOWS_STRING D_("_Windows")
#define GNOME_MENU_WINDOWS_PATH D_("_Windows/")
#endif
