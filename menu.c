#include "menu.h"
#include "derived.h"
#include "node.h"
#include "tree.h"
#include "compare.h"

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

void menu_set_view_defaults (GtkMenuShell *shell)
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

void menu_create (GnomeMDI *mdi, GnomeApp *app)
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

void file_open_cb (GtkWidget * widget, gpointer data)
{
	//main_diff();
}

void file_close_cb (GtkWidget * widget, gpointer data)
{
	g_print ("file_close_cb\n");
}

void view_cb (GtkWidget * widget, gpointer data)
{
	g_print ("view_cb\n");
}

void about_cb (GtkWidget * widget, gpointer data)
{
	g_print ("about_cb\n");
}

void show_cb (GtkWidget * widget, gpointer data)
{
	GnomeMDI *mdi = NULL;
	GtkWidget *w = NULL;
	GtkBin *bin = NULL;
	GtkWidget *child = NULL;
	GtkDiffTree *tree = NULL;
	GtkCList *list = NULL;
	GList *sel = NULL;
	GList *sel_end =NULL;
	GtkCListRow *row = NULL;
	//TreeNode *node = NULL;
	DiffTreeRow *dtr = NULL;
	GList *l2 = NULL;
	char *left = NULL;
	char *right = NULL;

	mdi = GNOME_MDI (data);
	w = gnome_mdi_get_active_view (mdi);
	bin = GTK_BIN (w);
	child = bin->child;
	tree = GTK_DIFF_TREE (child);
	list = GTK_CLIST (tree);

	//g_mem_chunk_print (list->row_mem_chunk);

	sel = list->selection;
	sel_end = list->selection_end;

	l2 = (GList*) (sel->data);
	row = (GtkCListRow*) (l2->data);

	dtr = (DiffTreeRow*) row;
	//node = row->data;

	/*
	g_print ("show_cb\n");
	g_print ("mdi = %p\n", mdi);
	g_print ("view = %p\n", w);
	g_print ("child = %p, %s\n", child, gtk_widget_get_name (GTK_WIDGET (child)));
	g_print ("l/r = %s/%s\n", tree->left, tree->right);
	g_print ("sel = %p\n", sel);

	g_print ("show_cb %s, %d, %s\n", dtr->name, dtr->status, dtr->path);
	*/
	//g_print ("show_cb %d\n", dtr->status);

	left  = g_strconcat (tree->left,  G_DIR_SEPARATOR_S, dtr->path, NULL);
	right = g_strconcat (tree->right, G_DIR_SEPARATOR_S, dtr->path, NULL);

	//g_print ("diff -u %s %s\n", left, right);
	compare (left, right);
}

