/* $Revision: 1.23 $ */

#include <gnome.h>
#include "mdi.h"
#include "menu.h"
#include "derived.h"
#include "compare.h"
#include "global.h"

/*----------------------------------------------------------------------------*/
static GList   * gd_mdi_create_menus (GnomeMDIChild * child, GtkWidget * view, gpointer data);
static GtkWidget * gd_mdi_create_compare_view (GnomeMDIChild * child, gpointer data);
static GtkWidget * gd_mdi_create_view (GnomeMDIChild * child, gpointer data);
static GtkWidget * gd_mdi_set_label (GnomeMDIChild * child, GtkWidget * old_label, gpointer data);
static gint remove_child (GnomeMDI *mdi, GnomeMDIChild *child);
static void app_created (GnomeMDI * mdi, GnomeApp * app);
static void destroy (GnomeMDI *mdi);

GnomeMDI * mdi_new (gchar *appname, gchar *title);
void mdi_add_diff (GnomeMDI *mdi, DiffOptions *diff);
/*----------------------------------------------------------------------------*/

static GtkWidget *
gd_mdi_create_compare_view (GnomeMDIChild * child, gpointer data)
{
	DiffOptions *diff    = data;
	GtkWidget   *compare = gtk_compare_new (diff);
	GtkWidget   *scroll  = gtk_scrolled_window_new (NULL, NULL);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (scroll), compare);

	gtk_widget_show_all (scroll);
	return scroll;
}

static GtkWidget *
gd_mdi_create_view (GnomeMDIChild * child, gpointer data)
{
	GtkWidget      *scroll = NULL;
	GtkWidget      *tree   = NULL;
	DiffOptions    *diff   = NULL;

	g_return_val_if_fail (child != NULL, NULL);
	g_return_val_if_fail (data  != NULL, NULL);

	scroll = gtk_scrolled_window_new (NULL, NULL);

	g_return_val_if_fail (scroll != NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	diff = data;
	tree = gtk_diff_tree_new (2, 0, diff);

	gtk_container_add (GTK_CONTAINER (scroll), tree);

	gtk_widget_show_all (scroll);
	return scroll;
}

static GtkWidget *
gd_mdi_set_label (GnomeMDIChild * child,
		    GtkWidget * old_label,
		    gpointer data)
{
	// This label will show up in both the notebook tab AND the window menu
	GtkWidget	*hbox   = NULL;
	GtkWidget	*pixmap = NULL;
	GtkWidget	*label  = NULL;
	DiffOptions	*diff   = NULL;

	if (old_label == NULL)						/* We have to create a new label */
	{
		diff = data;
		hbox = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new (g_strdup_printf ("%s", child->name));
		gtk_widget_show (label);
		//pixmap = gnome_stock_new_with_icon (GNOME_STOCK_MENU_BOOK_OPEN);
		if ((diff->type == Dir) || (diff->type == DirPatch))
		{
			pixmap = gtk_pixmap_new (pixmap_open, mask_open);
		}
		else
		{
			pixmap = gtk_pixmap_new (pixmap_leaf, mask_leaf);
		}
		gtk_widget_show (pixmap);
		gtk_box_pack_start (GTK_BOX (hbox), pixmap, FALSE, FALSE, 2);
		gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 2);
	}
	else
	{
		hbox = old_label;
	}

	return hbox;
}

static GList   *
gd_mdi_create_menus (GnomeMDIChild * child, GtkWidget * view, gpointer data)
{
	// These menus are mdi child specific -- we chose to distinguish between tree and file
	GList          *menu_list;
	GtkWidget      *menu, *w;
	DiffOptions *diff = NULL;

	diff = data;

	//g_print ("gd_mdi_create_menus (w = %s) (p = %s) (pp = %s) (ppp = %s) (pppp = %s)\n", gtk_widget_get_name (view), gtk_widget_get_name (view->parent), gtk_widget_get_name (view->parent->parent), gtk_widget_get_name (view->parent->parent->parent), gtk_widget_get_name (view->parent->parent->parent->parent));
	menu_list = NULL;

	/* the Child menu */
	menu = gtk_menu_new ();

	w = gtk_menu_item_new_with_label ("Add View");
	gtk_widget_show (w);

	gtk_menu_append (GTK_MENU (menu), w);
	w = gtk_menu_item_new_with_label ("Remove View");
	gtk_widget_show (w);

	gtk_menu_append (GTK_MENU (menu), w);

	if ((diff->type == Dir) || (diff->type == DirPatch))
	{
		w = gtk_menu_item_new_with_label ("Dir menu");
	}
	else
	{
		w = gtk_menu_item_new_with_label ("File menu");
	}
	gtk_widget_show (w);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (w), menu);
	menu_list = g_list_append (menu_list, w);

	return menu_list;
}

static void 
app_created (GnomeMDI * mdi, GnomeApp * app)
{
	//g_print ("app_created\n");
	//gnome_app_create_menus (app, main_menu);
	menu_create (mdi, app);
	gtk_window_set_default_size (GTK_WINDOW (app), 500, 500);
	gtk_widget_show_all (GTK_WIDGET (app));
}

static void
destroy (GnomeMDI *mdi)
{
	gtk_main_quit();
}

static gint
remove_child (GnomeMDI *mdi, GnomeMDIChild *child)
{
	//XXX ask config for confirm, if compare just close, if tree 'close all compares, too?'
	//g_print ("remove child\n");
	//XXX update menus
	return TRUE;			// yes let it die
}

GnomeMDI *
mdi_new (gchar *appname, gchar *title)
{
	GnomeMDI *mdi = NULL;

	mdi = GNOME_MDI (gnome_mdi_new (appname, title));

	mdi->tab_pos = GTK_POS_TOP;		// GTK_POS_LEFT, GTK_POS_RIGHT, GTK_POS_TOP, GTK_POS_BOTTOM
	gnome_mdi_set_mode (mdi, GNOME_MDI_NOTEBOOK); // GNOME_MDI_NOTEBOOK GNOME_MDI_TOPLEVEL GNOME_MDI_MODAL

	gtk_signal_connect (GTK_OBJECT (mdi), "app_created",  GTK_SIGNAL_FUNC (app_created),  NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "destroy",      GTK_SIGNAL_FUNC (destroy),      NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "remove_child", GTK_SIGNAL_FUNC (remove_child), NULL);

	//XXX move to menu.c
	gnome_mdi_set_child_menu_path(mdi, _("Compare"));		// child specific menus
	gnome_mdi_set_child_list_path(mdi, _("Children/"));		// automatic window list

	gnome_mdi_open_toplevel (mdi);

	return mdi;
}

void
mdi_add_diff (GnomeMDI *mdi, DiffOptions *diff)
{
	GnomeMDIGenericChild	*child = NULL;
	char			*name  = NULL;

	g_return_if_fail (mdi  != NULL);
	g_return_if_fail (diff != NULL);

	name  = g_strdup_printf ("%s\n%s", diff->left, diff->right);
	child = gnome_mdi_generic_child_new (name);

	g_return_if_fail (child != NULL);
	gnome_mdi_generic_child_set_menu_creator (child, gd_mdi_create_menus, diff);//XXX
	gnome_mdi_generic_child_set_label_func   (child, gd_mdi_set_label,    diff);

	if ((diff->type == Dir) || (diff->type == DirPatch))
	{
		gnome_mdi_generic_child_set_view_creator (child, gd_mdi_create_view,  diff);
	}
	else
	{
		gnome_mdi_generic_child_set_view_creator (child, gd_mdi_create_compare_view, diff);
	}

	gnome_mdi_add_child (mdi, GNOME_MDI_CHILD (child));			/* Add one child to the MDI */
	gnome_mdi_add_view  (mdi, GNOME_MDI_CHILD (child));			/* Display one view of that child */

	//XXX update menus
}

