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

/* $Revision: 1.38 $ */

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

/*----------------------------------------------------------------------------*/

// in this file there are callbacks - only get parms then delegate
static void about_cb          (GtkWidget *, GnomeMDI *); // routed to help.c gd_about()
static void close_view_cb     (GtkWidget *, GnomeMDI *); // etc
static void compare_cb        (GtkWidget *, GnomeMDI *);
static void contents_cb       (GtkWidget *, GnomeMDI *);
static void exit_gdiff_cb     (GtkWidget *, GnomeMDI *);
static void new_diff_cb       (GtkWidget *, GnomeMDI *);
static void next_diff_cb      (GtkWidget *, GnomeMDI *);
static void preferences_cb    (GtkWidget *, GnomeMDI *);
static void prev_diff_cb      (GtkWidget *, GnomeMDI *);
static void rescan_cb         (GtkWidget *, GnomeMDI *);
static void save_file_list_cb (GtkWidget *, GnomeMDI *);
static void status_bar_cb     (GtkWidget *, GnomeMDI *);
static void view_cb           (GtkWidget *, GnomeMDI *);

/*----------------------------------------------------------------------------*/
static void menu_set_view_defaults (GtkMenuShell *shell);
void menu_create (GnomeMDI *mdi, GnomeApp *app);
/*----------------------------------------------------------------------------*/

static GnomeUIInfo file_menu[] =
{
	{ GNOME_APP_UI_ITEM, "_New Difference", "", new_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_n, GDK_CONTROL_MASK, NULL },
	GNOMEUIINFO_SEPARATOR,
	{ GNOME_APP_UI_ITEM, "_Save File List", "", save_file_list_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_s, GDK_CONTROL_MASK, NULL },
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_MENU_CLOSE_ITEM(close_view_cb, NULL),
	GNOMEUIINFO_MENU_EXIT_ITEM(exit_gdiff_cb, NULL),
	GNOMEUIINFO_END
};

static GnomeUIInfo display_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "_Same", "Same tooltip", view_cb, GUINT_TO_POINTER (eFileSame), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F5, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Left", "Left tooltip", view_cb, GUINT_TO_POINTER (eFileLeft), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F6, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Right", "Right tooltip", view_cb, GUINT_TO_POINTER (eFileRight), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F7, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Different", "Different tooltip", view_cb, GUINT_TO_POINTER (eFileDiff), NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F8, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_TOGGLEITEM, "_Error", "Error tooltip", view_cb, GUINT_TO_POINTER (eFileError), NULL, GNOME_APP_PIXMAP_NONE, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo tree_style_list_menu[] =
{
	{ GNOME_APP_UI_ITEM, "Tree", "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F2, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "List (with full path)", "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F3, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "List (with no path)", "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F4, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo tree_style_menu[] =
{
	GNOMEUIINFO_RADIOLIST(tree_style_list_menu),
	GNOMEUIINFO_END
};

static GnomeUIInfo view_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "File _Count", "Different tooltip", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F8, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_SEPARATOR,
	{ GNOME_APP_UI_ITEM, "_Previous Difference", "", prev_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F9, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "_Next Difference",     "", next_diff_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_F10, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "Refresh", 	    "", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_r, GDK_CONTROL_MASK, NULL },
	GNOMEUIINFO_SEPARATOR,
	{ GNOME_APP_UI_SUBTREE_STOCK, "_Display", NULL, display_menu, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_SUBTREE_STOCK, "_Style", NULL, tree_style_menu, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo file_style_list_menu[] =
{
	{ GNOME_APP_UI_ITEM, "One pane",  "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F2, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_ITEM, "Two panes", "tooltip", NULL, NULL, NULL, (GnomeUIPixmapType) 0, NULL, GDK_F3, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo file_style_menu[] =
{
	GNOMEUIINFO_RADIOLIST(file_style_list_menu),
	GNOMEUIINFO_END
};

static GnomeUIInfo view_menu2[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "_Line Numbers", "Different tooltip", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_SEPARATOR,
	{ GNOME_APP_UI_ITEM, "Refresh", 	    "", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_r, GDK_CONTROL_MASK, NULL },
	{ GNOME_APP_UI_SUBTREE_STOCK, "_Style", NULL, file_style_menu, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_END
};

static GnomeUIInfo sort_list_menu[] =
{
	GNOMEUIINFO_RADIOITEM_DATA  ("_Alphabetical", NULL , NULL, NULL, NULL),
	GNOMEUIINFO_RADIOITEM_DATA  ("_By Diff Type", NULL , NULL, NULL, NULL),
	GNOMEUIINFO_END
};

static GnomeUIInfo sort_menu[] =
{
	{ GNOME_APP_UI_TOGGLEITEM, "_Directories first", "Different tooltip", NULL, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_RADIOLIST(sort_list_menu),
	GNOMEUIINFO_END
};

static GnomeUIInfo mdi_menu[] =
{
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
	GNOMEUIINFO_MENU_VIEW_TREE     (view_menu),
	//{ GNOME_APP_UI_SUBTREE_STOCK, "_View2", NULL, view_menu2, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	{ GNOME_APP_UI_SUBTREE_STOCK, "_Sort", NULL, sort_menu, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL },
	GNOMEUIINFO_MENU_SETTINGS_TREE (settings_menu),
	GNOMEUIINFO_MENU_WINDOWS_TREE  (windows_menu),
	GNOMEUIINFO_MENU_HELP_TREE     (help_menu),
	GNOMEUIINFO_END
};
	//GNOMEUIINFO_MENU_MISC_TREE     (misc_menu),

/*
File
	  New difference		CTRL-n		always
	  -------------
	  Save file list		CTRL-s		tree
	  -------------
	  Close				CTRL-w		tree | compare
	  Exit				CTRL-q		always

View (tree)						tree
	X File count					tree
	  -------------
	  Prev diff			F9		tree
	  Next diff			F10		tree
	  -------------
	> Display					tree
		X Same			F5		tree
		X Left			F6		tree
		X Right			F7		tree
		X Diff			F8		tree
		X Error					tree
	  Refresh			CTRL-r		tree
	> Style
		Tree			F2		tree
		List (path)		F3		tree
		List (no path)		F4		tree

View (file)						file
	X Line numbers					file
	  -------------
	  Refresh			CTRL-r		file
	> Style						file
		One pane		F2		file
		Two panes		F3		file

Sort							tree
	X Directories first				tree
	  -------------
	O Alphabetical			SHIFT-F9	tree
	O By diff type			SHIFT-F10	tree

Settings
	show status bar					always
	preferences					always

Windows
	window list.					tree | compare

Help
	contents					always
	about						always

________________________________________________________________________________

MDI							always
	O Default					always
	O Notebook					always
	O Modal						always
	O Top level					always

Misc
	Compare			tree
	rescan whole tree	tree
	Rescan from here	tree & selection
	Stop diff now		treediff
	save file list		tree
	save file list here	tree & selection
	display style tree	tree
*/

//______________________________________________________________________________
//
// Public

void
menu_create (GnomeMDI *mdi, GnomeApp *app)
{
	GtkWidget *status = NULL;

	gnome_app_create_menus_with_data (app, main_menu, mdi);

	menu_set_view_defaults (GTK_MENU_SHELL (app->menubar));

	status = gtk_statusbar_new();

	gnome_app_install_statusbar_menu_hints (GTK_STATUSBAR (status), main_menu);
	gnome_app_set_statusbar (GNOME_APP (app), gtk_statusbar_new());

	gnome_mdi_set_child_menu_path (mdi, GNOME_MENU_VIEW_PATH);	// child specific menus
	gnome_mdi_set_child_list_path (mdi, GNOME_MENU_WINDOWS_PATH);	// automatic window list
}

static void
menu_set_view_defaults (GtkMenuShell *shell)
{
#if 0
	GnomeUIInfo *window = NULL;
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

	//XXX window = get_menu_item_by_name ("_WINDOWS"); // safety
	//XXX also useful for en/disabling menu items
	window = &main_menu[4];
	gtk_widget_set_sensitive (window->widget, FALSE);
#endif
}

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
			gtk_window_set_transient_for (GTK_WINDOW (about), GTK_WINDOW (parent));
		}

		gtk_widget_show_all (about);
	}
}

static void
close_view_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	//XXX move the actual doing to mdi.c
	GnomeMDIChild *child = NULL;

	g_return_if_fail (mdi != NULL);
	g_return_if_fail (GNOME_IS_MDI (mdi));

	child = gnome_mdi_get_active_child (mdi);

	g_return_if_fail (child != NULL);

	gnome_mdi_remove_child (mdi, child, FALSE);
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

static void
contents_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	// NONE
	g_print ("contents_cb\n");
}

static void
exit_gdiff_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	g_print ("exit_gdiff_cb\n");
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

static void
next_diff_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	//mdichild - then I can figure out what the view is of
	g_print ("next_diff_cb\n");
	exclude_dialog(); //XXX temp
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
prev_diff_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	//mdichild - then I can figure out what the view is of
	g_print ("prev_diff_cb\n");
}

static void
rescan_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	//mdichild - then I can figure out what the view is of
	g_print ("rescan_cb\n");
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
view_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	//tree or mdichild or mdi
	g_print ("view_cb %p\n", gtk_object_get_data (GTK_OBJECT (widget), GNOMEUIINFO_KEY_UIDATA));
}

