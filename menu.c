#include "menu.h"
#include "file.h"

void file_open_cb	(GtkWidget * widget, gpointer data);
void file_close_cb	(GtkWidget * widget, gpointer data);
void view_cb		(GtkWidget * widget, gpointer data);
void about_cb		(GtkWidget * widget, gpointer data);

static GnomeUIInfo file_menu[] =
{
	GNOMEUIINFO_MENU_OPEN_ITEM  (file_open_cb,  NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM (file_close_cb, NULL),
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

static GnomeUIInfo help_menu[] =
{
	GNOMEUIINFO_MENU_ABOUT_ITEM (about_cb, NULL),
	GNOMEUIINFO_END
};

static GnomeUIInfo main_menu[] =
{
	GNOMEUIINFO_MENU_FILE_TREE (file_menu),
	GNOMEUIINFO_MENU_VIEW_TREE (view_menu),
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

void menu_create (GnomeApp *app)
{
	GtkWidget *status = NULL;

	g_return_if_fail (app != NULL);

	gnome_app_create_menus (app, main_menu);
	
	g_return_if_fail (app->menubar != NULL);

	menu_set_view_defaults (GTK_MENU_SHELL (app->menubar));

	status = gtk_statusbar_new();

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


