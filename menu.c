/* $Revision: 1.22 $ */

#include "config.h"
#include <gnome.h>
#include "menu.h"
#include "derived.h"
#include "node.h"
#include "tree.h"
#include "compare.h"
#include "mdi.h"

/*----------------------------------------------------------------------------*/
void menu_set_view_defaults (GtkMenuShell *shell);
void menu_create (GnomeMDI *mdi, GnomeApp *app);
void file_open_cb (GtkWidget * widget, gpointer data);
void file_close_cb (GtkWidget * widget, gpointer data);
void view_cb (GtkWidget * widget, gpointer data);
void about_cb (GtkWidget * widget, gpointer data);
GtkWidget * get_current_view (GnomeMDI *mdi);
DiffOptions * get_current_selection (GnomeMDI *mdi);
void show_cb (GtkWidget * widget, gpointer data);
/*----------------------------------------------------------------------------*/

void file_open_cb	(GtkWidget * widget, gpointer data);
void file_close_cb	(GtkWidget * widget, gpointer data);
void view_cb		(GtkWidget * widget, gpointer data);
void about_cb		(GtkWidget * widget, gpointer data);
void show_cb		(GtkWidget * widget, gpointer data);

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
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Identical"), N_("Show files that are the same"),           view_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F5, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Left"),      N_("Show files that only appear in 'left'"),  view_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F6, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Right"),     N_("Show files that only appear in 'right'"), view_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F7, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Different"), N_("Show files that are different"),          view_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F8, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo compare_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, N_("_Show diff"), N_("asfasdfShow files that are the same"),     show_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F5, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo help_menu[] =
{
	GNOMEUIINFO_MENU_ABOUT_ITEM (about_cb, NULL),
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

void
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

void
file_open_cb (GtkWidget * widget, gpointer data)
{
	//main_diff();
}

void
file_close_cb (GtkWidget * widget, gpointer data)
{
	g_print ("file_close_cb\n");
}

void
view_cb (GtkWidget * widget, gpointer data)
{
	g_print ("view_cb\n");
}

void
about_cb (GtkWidget * widget, gpointer data)
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

GtkWidget *
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

DiffOptions *
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

void
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

