/* $Revision: 1.24 $ */

#include "config.h"
#include <gnome.h>
#include "menu.h"
#include "derived.h"
#include "node.h"
#include "tree.h"
#include "compare.h"
#include "mdi.h"

// XXX method to automate menu creation, callbacks, signals and accelerators!

/*----------------------------------------------------------------------------*/
static DiffOptions * get_current_selection (GnomeMDI *mdi);
static GtkWidget * get_current_view (GnomeMDI *mdi);
static void menu_set_view_defaults (GtkMenuShell *shell);
static void about_orig_cb		(GtkWidget * widget, gpointer data);
static void file_close_cb	(GtkWidget * widget, gpointer data);
static void file_open_cb	(GtkWidget * widget, gpointer data);
static void show_cb		(GtkWidget * widget, gpointer data);
static void view_orig_cb		(GtkWidget * widget, gpointer data);

void menu_create (GnomeMDI *mdi, GnomeApp *app);
/*----------------------------------------------------------------------------*/

GtkStatusbar *global_statusbar = NULL;

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

void
menu_create (GnomeMDI *mdi, GnomeApp *app)
{
	GtkWidget *status = NULL;

	g_return_if_fail (app != NULL);

	gnome_app_create_menus_with_data (app, main_menu, mdi);
	
	g_return_if_fail (app->menubar != NULL);

	menu_set_view_defaults (GTK_MENU_SHELL (app->menubar));

	status = gtk_statusbar_new();
	global_statusbar = GTK_STATUSBAR (status);

	gnome_app_install_statusbar_menu_hints (GTK_STATUSBAR (status), main_menu);
	gnome_app_set_statusbar (GNOME_APP (app), status);

/*
	if these menus are created with data,
	then it must be a pointer to a tree
	will this ever happen?:

	data  -> tree
	!data -> MDI -> all trees?

	should view ident/diff/left/right be global or per tree?
*/
}

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
	GtkWidget      *about;
	const char     *authors[] =
	{"jim <jim@@ait.co.uk>", "bob <bob@@nrma.com.au>", "dave <dave@@vmware.com>", NULL};
	const char     *copyright = "Copyright 1999";
	const char     *extra = "Extra information";

	about = gnome_about_new (PACKAGE, VERSION, copyright, authors, extra, NULL);

	//gtk_window_set_transient_for (GTK_WINDOW (about), GTK_WINDOW (app));

	gtk_widget_show (about);

	return;
}

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

//______________________________________________________________________________
//

static void about_cb		(GtkWidget *widget, gpointer data) {}
static void close_view_cb	(GtkWidget *widget, gpointer data) {}
static void compare_cb		(GtkWidget *widget, gpointer data) {}
static void contents_cb		(GtkWidget *widget, gpointer data) {}
static void exit_gdiff_cb	(GtkWidget *widget, gpointer data) {}
static void new_diff_cb		(GtkWidget *widget, gpointer data) {}
static void next_diff_cb	(GtkWidget *widget, gpointer data) {}
static void preferences_cb	(GtkWidget *widget, gpointer data) {}
static void prev_diff_cb	(GtkWidget *widget, gpointer data) {}
static void rescan_cb		(GtkWidget *widget, gpointer data) {}
static void save_file_list_cb	(GtkWidget *widget, gpointer data) {}
static void status_bar_cb	(GtkWidget *widget, gpointer data) {}
static void view_cb		(GtkWidget *widget, gpointer data) {}

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

