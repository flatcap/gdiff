#include "menu.h"
#include "file.h"

void file_open_cb	(GtkWidget * widget, gpointer data);
void file_close_cb	(GtkWidget * widget, gpointer data);
void about_cb		(GtkWidget * widget, gpointer data);

GnomeUIInfo file_menu[] =
{
	GNOMEUIINFO_MENU_OPEN_ITEM  (file_open_cb,  NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM (file_close_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo help_menu[] =
{
	GNOMEUIINFO_MENU_ABOUT_ITEM (about_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo main_menu[] =
{
	GNOMEUIINFO_MENU_FILE_TREE (file_menu),
	GNOMEUIINFO_MENU_HELP_TREE (help_menu),
	GNOMEUIINFO_END
};

void menu_create (GnomeApp *app)
{
	g_return_if_fail (app != NULL);

	gnome_app_create_menus (app, main_menu);
#if 0
	if these menus are created with data,
	then it must be a pointer to a tree
	will this ever happen?:
#endif
}

void file_open_cb (GtkWidget * widget, gpointer data)
{
	main_diff();
}

void file_close_cb (GtkWidget * widget, gpointer data)
{
}

void about_cb (GtkWidget * widget, gpointer data)
{
}

