/* $Revision: 1.28 $ */

#include "config.h"
#include <gnome.h>
#include "menu.h"
#include "derived.h"
#include "node.h"
#include "tree.h"
#include "compare.h"
#include "mdi.h"
#include "file.h"

// in this file there are callbacks - only get parms then delegate
static void about_cb		(GtkWidget *widget, gpointer data); // routed to help.c gd_about()
static void close_view_cb	(GtkWidget *widget, gpointer data); // etc
static void compare_cb		(GtkWidget *widget, gpointer data);
static void contents_cb		(GtkWidget *widget, gpointer data);
static void exit_gdiff_cb	(GtkWidget *widget, gpointer data);
static void new_diff_cb		(GtkWidget *widget, gpointer data);
static void next_diff_cb	(GtkWidget *widget, gpointer data);
static void preferences_cb	(GtkWidget *widget, gpointer data);
static void prev_diff_cb	(GtkWidget *widget, gpointer data);
static void rescan_cb		(GtkWidget *widget, gpointer data);
static void save_file_list_cb	(GtkWidget *widget, gpointer data);
static void status_bar_cb	(GtkWidget *widget, gpointer data);
static void view_cb		(GtkWidget *widget, gpointer data);
/*----------------------------------------------------------------------------*/

#define DIFF_MENU		N_("_Diff")
#define GNOME_MENU_DIFF_STRING	D_("_Diff")
#define GNOME_MENU_DIFF_PATH	D_("_Diff/")
#define GNOMEUIINFO_MENU_DIFF_TREE(tree) { GNOME_APP_UI_SUBTREE_STOCK, DIFF_MENU, NULL, tree, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL }

/*----------------------------------------------------------------------------*/
static void menu_set_view_defaults (GtkMenuShell *shell);
void menu_create (GnomeMDI *mdi, GnomeApp *app);
/*----------------------------------------------------------------------------*/

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

//XXX move to MDI
GtkWidget *
get_toplevel_widget (GnomeMDI *mdi)
{
	return NULL;
}

//XXX move this method to MDI
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
	// XXX ask the tree for the selection -- it only allows 1!
	// XXX move this method to the tree
	// XXX link the TreeNodes to the DataNodes and vice versa
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
	list = (selection->data);

	g_return_val_if_fail (list != NULL, NULL);
	treerow = (DiffTreeRow*) list->data;

	g_return_val_if_fail (treerow != NULL, NULL);
	options = diffoptions_new();

	g_return_val_if_fail (options != NULL, NULL);
	options->left  = g_strconcat (tree->left,  G_DIR_SEPARATOR_S, treerow->path, NULL);
	options->right = g_strconcat (tree->right, G_DIR_SEPARATOR_S, treerow->path, NULL);
	options->type  = File;

	return options;
}

static void
close_view_cb (GtkWidget *widget, gpointer data)
{
	//MDIChild or MDI and current view
	g_print ("close_view_cb\n");
}

static void
compare_cb (GtkWidget *widget, gpointer data)
{
	//tree, mdichild or mdi
	//g_print ("compare_cb\n");

	GnomeMDI    *mdi  = NULL;
	DiffOptions *diff = NULL;

	mdi = GNOME_MDI (data);
	g_return_if_fail (mdi != NULL);

	diff = get_current_selection (mdi);

	g_return_if_fail (diff != NULL);
	mdi_add_diff (mdi, diff);
}

static void
contents_cb (GtkWidget *widget, gpointer data)
{
	// NONE
	g_print ("contents_cb\n");
}

static void
exit_gdiff_cb (GtkWidget *widget, gpointer data)
{
	//mdi
	g_print ("exit_gdiff_cb\n");
}

static void
next_diff_cb (GtkWidget *widget, gpointer data)
{
	//mdichild - then I can figure out what the view is of
	g_print ("next_diff_cb\n");
}

static void
preferences_cb (GtkWidget *widget, gpointer data)
{
	// NONE
	g_print ("preferences_cb\n");
}

static void
prev_diff_cb (GtkWidget *widget, gpointer data)
{
	//mdichild - then I can figure out what the view is of
	g_print ("prev_diff_cb\n");
}

static void
rescan_cb (GtkWidget *widget, gpointer data)
{
	//mdichild - then I can figure out what the view is of
	g_print ("rescan_cb\n");
}

static void
save_file_list_cb (GtkWidget *widget, gpointer data)
{
	// tree or mdichild or mdi
	g_print ("save_file_list_cb\n");
}

static void
status_bar_cb (GtkWidget *widget, gpointer data)
{
	g_print ("status_bar_cb\n");
}

static void
view_cb (GtkWidget *widget, gpointer data)
{
	//tree or mdichild or mdi
	g_print ("view_cb\n");
}

static void
new_diff_cb (GtkWidget *widget, gpointer data)
{
	//mdi app(window)
	new_file();
}

static void
about_cb (GtkWidget *widget, gpointer data)
{
	//app (window)
	static GtkWidget *about     = NULL;
	const char       *authors[] = { "Richard Russon <Richard.Russon@ait.co.uk>", NULL };
	const char       *copyright = "Copyright 1999";
	const char       *extra     = NULL;

	if (about)
	{
		gdk_window_show  (about->window);
		gdk_window_raise (about->window);
	}
	else
	{
		about = gnome_about_new (PACKAGE, VERSION, copyright, authors, extra, NULL);

		gtk_signal_connect (GTK_OBJECT (about), "destroy", GTK_SIGNAL_FUNC (gtk_widget_destroyed), &about);

		//gtk_window_set_transient_for (GTK_WINDOW (about), GTK_WINDOW (app));

		gtk_widget_show_all (about);
	}
}

static void
menu_set_view_defaults (GtkMenuShell *shell)
{
	GtkCheckMenuItem *item = NULL;
	int i;

	for (i = 0; i < 4; i++)
	{
		item = GTK_CHECK_MENU_ITEM (view_menu[i].widget);
		if (item)
		{
			item->active = TRUE;
		}
	}
}

void
menu_create (GnomeMDI *mdi, GnomeApp *app)
{
	GtkWidget *status = NULL;
	GnomeUIInfo *window = NULL;

	//GtkMenuBar *menubar = NULL;

	gnome_app_create_menus_with_data (app, main_menu, mdi);

	menu_set_view_defaults (GTK_MENU_SHELL (app->menubar));

	status = gtk_statusbar_new();

	gnome_app_install_statusbar_menu_hints (GTK_STATUSBAR (status), main_menu);
	gnome_app_set_statusbar (GNOME_APP (app), status);

	window = &main_menu[4];

	gtk_widget_set_sensitive (window->widget, FALSE);

/* XXX
GtkWidget*
gtk_widget_get_ancestor (GtkWidget *widget,
			 GtkType    widget_type)
GtkWidget*
gtk_widget_get_toplevel (GtkWidget *widget)
*/

	/*
	GtkWidget *status = NULL;

	g_return_if_fail (app != NULL);

	gnome_app_create_menus_with_data (app, main_menu, mdi);

	g_return_if_fail (app->menubar != NULL);

	menu_set_view_defaults (GTK_MENU_SHELL (app->menubar));

	status = gtk_statusbar_new();

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

