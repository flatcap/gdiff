/* $Revision: 1.30 $ */

#include "config.h"
#include <gnome.h>
#include "menu.h"
#include "derived.h"
#include "node.h"
#include "tree.h"
#include "compare.h"
#include "mdi.h"
#include "file.h"

/*----------------------------------------------------------------------------*/

#define DIFF_MENU		N_("_Diff")
#define GNOME_MENU_DIFF_STRING	D_("_Diff")
#define GNOME_MENU_DIFF_PATH	D_("_Diff/")
#define GNOMEUIINFO_MENU_DIFF_TREE(tree) { GNOME_APP_UI_SUBTREE_STOCK, DIFF_MENU, NULL, tree, NULL, NULL, (GnomeUIPixmapType) 0, NULL, 0, (GdkModifierType) 0, NULL }

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
	GNOMEUIINFO_MENU_DIFF_TREE     (diff_menu),
	GNOMEUIINFO_MENU_VIEW_TREE     (view_menu),
	GNOMEUIINFO_MENU_SETTINGS_TREE (settings_menu),
	GNOMEUIINFO_MENU_WINDOWS_TREE  (windows_menu),
	GNOMEUIINFO_MENU_HELP_TREE     (help_menu),
	GNOMEUIINFO_END
};

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

}

static void
menu_set_view_defaults (GtkMenuShell *shell)
{
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
	//mdi
	g_print ("exit_gdiff_cb\n");
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
}

static void
preferences_cb (GtkWidget *widget, GnomeMDI *mdi)
{
	static GtkWidget *prefs  = NULL;
	GnomeApp         *parent = NULL;

	g_return_if_fail (mdi != NULL);
	g_return_if_fail (GNOME_IS_MDI (mdi));

	if (prefs)
	{
		gdk_window_show  (prefs->window);
		gdk_window_raise (prefs->window);
	}
	else
	{
		parent = gnome_mdi_get_active_window (mdi);
		if (parent)
		{
			prefs = NULL;// get_prefs (GTK_WINDOW (parent));
			if (prefs)
			{
				gtk_signal_connect (GTK_OBJECT (prefs), "destroy", GTK_SIGNAL_FUNC (gtk_widget_destroyed), &prefs);
				gtk_widget_show_all (prefs);
			}
		}
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
	// tree or mdichild or mdi
	g_print ("save_file_list_cb\n");
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

