/*
    menu.c -- description
    Copyright (C) 1999  Richard Russon <richard.russon@ait.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Revision: 1.46 $ */

#include "config.h"
#include <gnome.h>
#include "menu.h"
#include "derived.h"
#include "node.h"
#include "tree.h"
#include "compare.h"
#include "mdi.h"
#include "file.h"
#include "exclude.h" //XXX temp

// Private data stolen from the MDI
#define GNOME_MDI_MENUBAR_INFO_KEY           "MDIMenubarUIInfo"
#define GNOME_MDI_CHILD_MENU_INFO_KEY        "MDIChildMenuUIInfo"

/*______________________________________________________________________________
*/

// in this file there are callbacks - only get parms then delegate
static void about_cb          (GtkWidget *, GnomeMDI *);	// help.c	gd_about()
static void close_display_cb  (GtkWidget *, GnomeMDI *);	// mdi.c	mdi_close_view()
static void compare_cb        (GtkWidget *, GnomeMDI *);	//
static void contents_cb       (GtkWidget *, GnomeMDI *);	// help.c	
static void exit_gdiff_cb     (GtkWidget *, GnomeMDI *);	// mdi.c	mdi_close_view()
static void new_diff_cb       (GtkWidget *, GnomeMDI *);	// file.c	new_file()
static void preferences_cb    (GtkWidget *, GnomeMDI *);	// options.c	
static void save_file_list_cb (GtkWidget *, GnomeMDI *);	//
static void status_bar_cb     (GtkWidget *, GnomeMDI *);	//

static void display_cb        (GtkWidget *, GnomeMDIChild *);	//XXX this will be MDIDiffChild
static void file_count_cb     (GtkWidget *, GnomeMDIChild *);	//
static void line_numbers_cb   (GtkWidget *, GnomeMDIChild *);	//
static void next_diff_cb      (GtkWidget *, GnomeMDIChild *);	//
static void prev_diff_cb      (GtkWidget *, GnomeMDIChild *);	//
static void refresh_cb        (GtkWidget *, GnomeMDIChild *);	//
static void style_cb          (GtkWidget *, GnomeMDIChild *);	//


/*----------------------------------------------------------------------------*/
//static void menu_set_view_defaults (GtkMenuShell *shell);
void menu_create (GnomeMDI *mdi, GnomeApp *app);
/*----------------------------------------------------------------------------*/

static GnomeUIInfo main_menu[];				// Main menu
static GnomeUIInfo   file_menu[];
static GnomeUIInfo   settings_menu[];
static GnomeUIInfo   windows_menu[];
static GnomeUIInfo   help_menu[];

static GnomeUIInfo list_main_menu[];			// MDI specific menu for lists
static GnomeUIInfo   list_view_menu[];
static GnomeUIInfo     list_display_menu[];
static GnomeUIInfo     list_style_menu[];
static GnomeUIInfo       list_style_radio_menu[];
static GnomeUIInfo   list_sort_menu[];
static GnomeUIInfo     list_sort_radio_menu[];

static GnomeUIInfo file_main_menu[];			// MDI specific menu for files
static GnomeUIInfo   file_list_view_menu[];
static GnomeUIInfo     file_style_menu[];
static GnomeUIInfo       file_style_radio_menu[];

/*----------------------------------------------------------------------------*/

static GnomeUIInfo main_menu[] =
{
	GNOMEUIINFO_MENU_FILE_TREE     (file_menu),
	GNOMEUIINFO_MENU_SETTINGS_TREE (settings_menu),
	GNOMEUIINFO_MENU_WINDOWS_TREE  (windows_menu),
	GNOMEUIINFO_MENU_HELP_TREE     (help_menu),
	GNOMEUIINFO_END
};

static GnomeUIInfo file_menu[] =
{
	{ GNOME_APP_UI_ITEM, "_New Difference", "", new_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_n, GDK_CONTROL_MASK, NULL },
	{ GNOME_APP_UI_ITEM, "_Compare", "", compare_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_c, GDK_CONTROL_MASK, NULL },
	GNOMEUIINFO_SEPARATOR,
	{ GNOME_APP_UI_ITEM, "_Save File List", "", save_file_list_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_s, GDK_CONTROL_MASK, NULL },
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_MENU_CLOSE_ITEM(close_display_cb, NULL),
	GNOMEUIINFO_MENU_EXIT_ITEM(exit_gdiff_cb, NULL),
	GNOMEUIINFO_END
};

static GnomeUIInfo settings_menu[] =
{
	GNOMEUIINFO_TOGGLEITEM_DATA ("Show Status _Bar", NULL, status_bar_cb, NULL, NULL), // ctrl-b
	GNOMEUIINFO_MENU_PREFERENCES_ITEM (preferences_cb, NULL),	// ctrl-p
	GNOMEUIINFO_END
};

static GnomeUIInfo windows_menu[] =
{
	GNOMEUIINFO_END
};

static GnomeUIInfo help_menu[] =
{
	GNOMEUIINFO_ITEM_NONE ("_Contents", NULL, contents_cb),		// F1
	GNOMEUIINFO_MENU_ABOUT_ITEM(about_cb, NULL),			// F2
	GNOMEUIINFO_END
};

/*______________________________________________________________________________
*/

static GnomeUIInfo list_main_menu[] =
{
	GNOMEUIINFO_MENU_VIEW_TREE     (list_view_menu),
	{ GNOME_APP_UI_SUBTREE_STOCK, "_Sort", NULL, list_sort_menu, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo list_view_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "File _Count", "Different tooltip", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F8, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_SEPARATOR,
	{ GNOME_APP_UI_ITEM, "_Previous Difference", "", prev_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F9, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "_Next Difference",     "", next_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F10, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "Refresh", 	    "", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_r, GDK_CONTROL_MASK, NULL },
	GNOMEUIINFO_SEPARATOR,
	{ GNOME_APP_UI_SUBTREE_STOCK, "_Display", NULL, list_display_menu, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_SUBTREE_STOCK, "_Style", NULL, list_style_menu, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo list_display_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "_Same", "Same tooltip", display_cb, GUINT_TO_POINTER (eFileSame), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F5, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Left", "Left tooltip", display_cb, GUINT_TO_POINTER (eFileLeft), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F6, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Right", "Right tooltip", display_cb, GUINT_TO_POINTER (eFileRight), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F7, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Different", "Different tooltip", display_cb, GUINT_TO_POINTER (eFileDiff), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F8, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Error", "Error tooltip", display_cb, GUINT_TO_POINTER (eFileError), NULL, GNOME_APP_PIXMAP_NONE, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo list_style_menu[] =
{
	GNOMEUIINFO_RADIOLIST(list_style_radio_menu),
	GNOMEUIINFO_END
};

static GnomeUIInfo list_style_radio_menu[] =
{
	{ GNOME_APP_UI_ITEM, "Tree", "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F2, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "List (with full path)", "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F3, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "List (with no path)", "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F4, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo list_sort_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "_Directories first", "Different tooltip", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_RADIOLIST(list_sort_radio_menu),
	GNOMEUIINFO_END
};

static GnomeUIInfo list_sort_radio_menu[] =
{
	GNOMEUIINFO_RADIOITEM_DATA  ("_Alphabetical", NULL , NULL, NULL, NULL),
	GNOMEUIINFO_RADIOITEM_DATA  ("_By Diff Type", NULL , NULL, NULL, NULL),
	GNOMEUIINFO_END
};

/*______________________________________________________________________________
*/

static GnomeUIInfo file_main_menu[] =
{
	GNOMEUIINFO_MENU_VIEW_TREE     (file_list_view_menu),
	GNOMEUIINFO_END
};

static GnomeUIInfo file_list_view_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "_Line Numbers", "Different tooltip", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_SEPARATOR,
	{ GNOME_APP_UI_ITEM, "_Previous Difference", "", prev_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F9, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "_Next Difference",     "", next_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F10, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "Refresh", 	    "", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_r, GDK_CONTROL_MASK, NULL },
	{ GNOME_APP_UI_SUBTREE_STOCK, "_Style", NULL, file_style_menu, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo file_style_menu[] =
{
	GNOMEUIINFO_RADIOLIST(file_style_radio_menu),
	GNOMEUIINFO_END
};

static GnomeUIInfo file_style_radio_menu[] =
{
	{ GNOME_APP_UI_ITEM, "One pane",  "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F2, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "Two panes", "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F3, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

//______________________________________________________________________________
//
// Public

void
menu_create_main (GnomeMDI *mdi)
{
	gnome_mdi_set_menubar_template (mdi, main_menu);
}

void
menu_create (GnomeMDI *mdi, GnomeApp *app)
{
	GtkWidget *status = NULL;

	/*
	gnome_app_create_menus_with_data (app, main_menu, mdi);

	menu_set_view_defaults (GTK_MENU_SHELL (app->menubar));
	*/
	status = gtk_statusbar_new();

	//XXX need to point at the app object data (the copy created by the mdi)
	//gnome_app_install_statusbar_menu_hints (GTK_STATUSBAR (status), main_menu);
	// we can do this as soon as we get app_created
	// hang on... we're being CALLED from app_created

	gnome_app_set_statusbar (GNOME_APP (app), gtk_statusbar_new());

	gnome_mdi_set_child_menu_path (mdi, GNOME_MENU_FILE_STRING);	// child specific menus
	gnome_mdi_set_child_list_path (mdi, GNOME_MENU_WINDOWS_PATH);	// automatic window list
}

void
set_menu_for_view (MDIDiffChild *child)
{
	GnomeUIInfo *menu = NULL;
	DiffOptions *diff = child->diff_options;
	
	if ((diff->type == Dir) ||
	    (diff->type == DirPatch))
	{
		menu = list_main_menu;
	}
	else
	{
		menu = file_main_menu;
	}

	gnome_mdi_child_set_menu_template (GNOME_MDI_CHILD (child), menu);
}

/*
static void
menu_set_view_defaults (GtkMenuShell *shell)
{
#if 0
	GnomeUIInfo *window = NULL;
	GtkCheckMenuItem *item = NULL;
	int i;

	for (i = 0; i < 4; i++)
	{
		item = GTK_CHECK_MENU_ITEM (list_view_menu[i].widget);
		if (item)
		{
			item->active = TRUE;
		}
	}

	//XXX window = get_menu_item_by_name ("_WINDOWS"); // safety
	//XXX also useful for en/disabling menu items
	window = &main_menu[4];
	gtk_widget_set_sensitive (window->widget, FALSE);
#endif
	{
		GtkWidget *w = (&main_menu[1])->widget;
		g_print ("type = %d\n", GTK_OBJECT_TYPE (GTK_OBJECT (w)));
		g_print ("item = %d\n", gtk_menu_item_get_type());
		g_print ("menu = %d\n", gtk_menu_get_type());
		g_print ("name = %smenu_get_type\n", gtk_widget_get_name (w));
	}
}
*/

//______________________________________________________________________________
//
// Private

static void
about_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	static GtkWidget *about     = NULL;
	const char       *authors[] = { "Richard Russon <Richard.Russon@ait.co.uk>", NULL };
	const char       *copyright = "Copyright 1999";
	const char       *extra     = NULL;
	GnomeApp         *parent    = NULL;

	g_return_if_fail (mdi != NULL);
	g_return_if_fail (GNOME_IS_MDI (mdi));

	//XXX race condition to set about?
	if (about)
	{
		gdk_window_show  (about->window);
		gdk_window_raise (about->window);
	}
	else
	{
		about = gnome_about_new (PACKAGE, VERSION, copyright, authors, extra, NULL);

		gtk_signal_connect (GTK_OBJECT (about), "destroy", GTK_SIGNAL_FUNC (gtk_widget_destroyed), &about);

		parent = gnome_mdi_get_active_window (mdi);
		if (parent)
		{
			gnome_dialog_set_parent (GNOME_DIALOG (about), GTK_WINDOW (parent));
		}

		gtk_widget_show_all (about);
	}
}

static void
close_display_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	GnomeMDIChild *child = NULL;

	g_return_if_fail (mdi != NULL);
	g_return_if_fail (GNOME_IS_MDI (mdi));

	child = gnome_mdi_get_active_child (mdi);

	g_return_if_fail (child != NULL);

	mdi_close_view (mdi, child);
}

static void
compare_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	//tree, mdichild or mdi
	//g_print ("compare_cb\n");

	DiffOptions *diff = NULL;
	GtkDiffTree *tree = NULL;

	g_return_if_fail (mdi != NULL);

	tree = GTK_DIFF_TREE (mdi_get_current_view (mdi));

	g_return_if_fail (tree != NULL);
	diff = get_current_selection (tree);

	g_return_if_fail (diff != NULL);
	mdi_add_diff (mdi, diff);
}

void
recurse_menu (GtkMenuShell *shell, int depth)
{
	static char *space = "                                            ";
	GList *list = NULL;
	GtkWidget *item = NULL;
	GtkWidget *submenu = NULL;

	if (!shell)
		return;

	list = shell->children;
	while (list)
	{
		item = GTK_WIDGET (list->data);
		g_print ("%.*s%s\n", depth, space, gtk_widget_get_name (item));

		submenu = GTK_MENU_ITEM (item)->submenu;
		if (submenu)
		{
			recurse_menu (GTK_MENU_SHELL (submenu), depth + 4);
		}

		list = list->next;
	}
}

void
recurse_template (GnomeUIInfo *info)
{
	g_print ("label  = %s\n", info[0].label);
	g_print ("widget = %p\n", info[0].widget);
}

void
dump_templates (GnomeApp *app)
{
	GnomeUIInfo *menu  = NULL;
	GnomeUIInfo *child = NULL;

	// OK, so this is actually private data of the MDI
	// We CAN get all this info by more orthodox routes,
	// but it takes a lot longer (we have to recurse through
	// all the menus / menu items and filter out all the junk).
	menu  = gtk_object_get_data (GTK_OBJECT (app), GNOME_MDI_MENUBAR_INFO_KEY);
	child = gtk_object_get_data (GTK_OBJECT (app), GNOME_MDI_CHILD_MENU_INFO_KEY);

	g_print ("menu = %p, child = %p\n", menu, child);

	recurse_template (menu);
	recurse_template (child);
}

void
dump_menu (GnomeApp *app)
{
	recurse_menu (GTK_MENU_SHELL (app->menubar), 0);
	dump_templates (app);
}

static void
contents_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	// NONE
	//g_print ("contents_cb\n");
	dump_menu (gnome_mdi_get_active_window (mdi));
}

static void
exit_gdiff_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	mdi_close (mdi);
}

static void
new_diff_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	GtkWindow *parent = NULL;

	g_return_if_fail (mdi != NULL);

	parent = GTK_WINDOW (gnome_mdi_get_active_window (mdi));

	new_file (mdi, parent);
}

static gint option = TRUE;

static void
next_diff_cb (GtkWidget *widget, GnomeMDIChild *child)
{
	//mdichild - then I can figure out what the view is of
	g_print ("next_diff_cb\n");
	//exclude_dialog(); //XXX temp
	option = TRUE;
}

static void
preferences_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	GnomeApp *parent = NULL;

	g_return_if_fail (mdi != NULL);
	g_return_if_fail (GNOME_IS_MDI (mdi));

	parent = gnome_mdi_get_active_window (mdi);
	if (parent)
	{
		show_preferences (GTK_WINDOW (parent), PageDefault);
	}
}

static void
toggle_option (GtkCheckButton *button, gint *option)
{
	g_return_if_fail (option != NULL);

	(*option) = !(*option);
}

gboolean
confirm (gchar *text, gint *option, GtkWindow *parent)
{
	GnomeDialog *msgbox = NULL;
	GtkWidget   *check  = NULL;
	gboolean     result = TRUE;			// by default, do nothing

	g_return_val_if_fail (text != NULL, TRUE);

	if ((option == NULL) ||
	   (*option == TRUE))
	{
		msgbox = GNOME_DIALOG (gnome_message_box_new (text,
							      GNOME_MESSAGE_BOX_QUESTION,
							      GNOME_STOCK_BUTTON_YES,
							      GNOME_STOCK_BUTTON_NO,
							      NULL));

		gtk_window_set_modal (GTK_WINDOW (msgbox), TRUE);
		gnome_dialog_set_default (msgbox, 1);

		if (option != NULL)
		{
			check = gtk_check_button_new_with_label ("(Don't ask me this question next time)");
			gtk_widget_show (check);
			gtk_box_pack_start (GTK_BOX (msgbox->vbox), check, FALSE, FALSE, 0);

			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), (*option == 0));
			gtk_signal_connect (GTK_OBJECT (check), "toggled", toggle_option, option);
		}

		if (parent)
		{
			gnome_dialog_set_parent (msgbox, parent);
		}

		result = (GNOME_YES == gnome_dialog_run_and_close (msgbox));
	}

	return result;
}
static void
prev_diff_cb (GtkWidget *widget, GnomeMDIChild *child)
{
	//mdichild - then I can figure out what the view is of
	//g_print ("prev_diff_cb\n");
	GnomeApp *app = NULL;

	app = GNOME_MDI (child->parent)->active_window;

	//XXX ought to ask the child for the app
	if (confirm ("Are you sure you want to quit?", &option, GTK_WINDOW (app)))
	{
		g_print ("QUIT\n");
	}
	else
	{
		g_print ("Do nothing\n");
	}
}

static void
save_file_list_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	GtkDiffTree *tree = NULL;

	tree = GTK_DIFF_TREE (mdi_get_current_view (mdi));
	if (tree)
	{
		gtk_diff_tree_save_list (tree);
	}
}

static void
status_bar_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	g_print ("status_bar_cb\n");
}

static void
set_state_tree_menu (GnomeUIInfo *menu)
{
	GtkWidget *sort = NULL;

	g_return_if_fail (menu != NULL);

	sort = menu[1].widget;

	gtk_widget_set_sensitive (sort, FALSE);	//XXX NYI
}

static void
set_state_file_menu (GnomeUIInfo *menu)
{
}

void
set_menu_state (GnomeMDI *mdi)
//XXX see who calls this, what vars they have, and decide what the best params are
//XXX make it so that this is only called by MDI/Child and after MDI signals
{
	gboolean     tree         = FALSE;
	gboolean     compare      = FALSE;
	GnomeApp    *app          = NULL;
	GtkWidget   *view         = NULL;
	GtkWidget   *bin          = NULL;
	GnomeUIInfo *main_menu    = NULL;
	GnomeUIInfo *child_menu   = NULL;
	GnomeUIInfo *file_menu    = NULL;
	GtkWidget   *file_compare = NULL;
	GtkWidget   *file_save    = NULL;
	GtkWidget   *file_close   = NULL;
	GtkWidget   *windows      = NULL;

	g_return_if_fail (mdi != NULL);

	app  = mdi->active_window;
	bin  = mdi->active_view;
	if (bin)
	{
		view = GTK_BIN (bin)->child;

		tree    = GTK_IS_DIFF_TREE (view);
		compare = GTK_IS_COMPARE   (view);
	}

	main_menu  = gtk_object_get_data (GTK_OBJECT (app), GNOME_MDI_MENUBAR_INFO_KEY);
	child_menu = gtk_object_get_data (GTK_OBJECT (app), GNOME_MDI_CHILD_MENU_INFO_KEY);

	file_menu = main_menu[0].moreinfo;
	windows   = main_menu[2].widget;

	g_return_if_fail (file_menu);
	g_return_if_fail (windows);

	file_compare  = file_menu[1].widget;
	file_save     = file_menu[3].widget;
	file_close    = file_menu[5].widget;

	gtk_widget_set_sensitive (windows,      (tree || compare));
	gtk_widget_set_sensitive (file_compare, tree);
	gtk_widget_set_sensitive (file_save,    tree);
	gtk_widget_set_sensitive (file_close,   (tree || compare));

	if (tree)
	{
		set_state_tree_menu (child_menu);
	}
	else
	{
		set_state_file_menu (child_menu);
	}
}

static void
display_cb (GtkWidget *widget, GnomeMDIChild *child)
{
	//tree or mdichild or mdi
	g_print ("display_cb %p\n", gtk_object_get_data (GTK_OBJECT (widget), GNOMEUIINFO_KEY_UIDATA));
	if (0)
	{
		//XXX dummies
		file_count_cb (NULL, NULL);
		line_numbers_cb (NULL, NULL);
		refresh_cb (NULL, NULL);
		style_cb (NULL, NULL);
	}
}

static void
file_count_cb (GtkWidget *widget, GnomeMDIChild *child)
{
	g_print ("file_count_cb\n");
}

static void
line_numbers_cb (GtkWidget *widget, GnomeMDIChild *child)
{
	g_print ("line_numbers_cb\n");
}

static void
refresh_cb (GtkWidget *widget, GnomeMDIChild *child)
{
	g_print ("refresh_cb\n");
}

static void
style_cb (GtkWidget *widget, GnomeMDIChild *child)
{
	g_print ("style_cb\n");
}

