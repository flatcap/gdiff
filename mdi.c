#include <gnome.h>
#include "options.h"
#include "mdi.h"
#include "menu.h"
#include "derived.h"
#include "compare.h"
#include "global.h"

#if 0
typedef GtkWidget *(*GnomeMDIChildViewCreator) (GnomeMDIChild *, gpointer);
typedef GList     *(*GnomeMDIChildMenuCreator) (GnomeMDIChild *, GtkWidget *, gpointer);
typedef gchar     *(*GnomeMDIChildConfigFunc)  (GnomeMDIChild *, gpointer);
typedef GtkWidget *(*GnomeMDIChildLabelFunc)   (GnomeMDIChild *, GtkWidget *, gpointer);
#endif

gint 
tree_compare (GtkCList * clist, gconstpointer ptr1, gconstpointer ptr2)
{
	const GtkCTreeRow *row1 = ptr1;
	const GtkCTreeRow *row2 = ptr2;

	int leaf1 = (row1->is_leaf);
	int leaf2 = (row2->is_leaf);

	char *text1 = GTK_CELL_PIXTEXT (row1->row.cell[clist->sort_column])->text;
	char *text2 = GTK_CELL_PIXTEXT (row2->row.cell[clist->sort_column])->text;

	//XXX check options clist is our tree obj, we can get the options from there
	if (leaf1 != leaf2)							// One leaf and one node
	{
		return ((leaf1) ? 1 : -1);
	}
	else
	{
		return strcmp (text1, text2);
	}
}

GtkWidget *
my_child_create_compare_view (GnomeMDIChild * child, gpointer data)
{
	DiffOptions *diff    = data;
	GtkWidget   *compare = gtk_compare_new (diff);
	GtkWidget   *scroll  = gtk_scrolled_window_new (NULL, NULL);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (scroll), compare);

	gtk_widget_show_all (scroll);
	return scroll;
}

GtkWidget *
my_child_create_view (GnomeMDIChild * child, gpointer data)
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

	/*XXX move this into the derived code*/
	gtk_clist_set_selection_mode     (GTK_CLIST (tree), GTK_SELECTION_BROWSE);
	gtk_clist_set_auto_sort          (GTK_CLIST (tree), TRUE);
	gtk_clist_set_compare_func       (GTK_CLIST (tree), tree_compare);
	gtk_clist_set_column_auto_resize (GTK_CLIST (tree), 0, TRUE);
	gtk_clist_column_titles_passive  (GTK_CLIST (tree));

	gtk_container_add (GTK_CONTAINER (scroll), tree);

	gtk_widget_show_all (scroll);
	return scroll;
}

GtkWidget *
my_child_set_label (GnomeMDIChild * child,
		    GtkWidget * old_label,
		    gpointer data)
{
	// This label will show up in both the notebook tab AND the window menu
	GtkWidget	*hbox   = NULL;
	GtkWidget	*pixmap = NULL;
	GtkWidget	*label  = NULL;
	DiffOptions	*diff   = NULL;

	diff = data;

	//g_print ("my_child_set_label\n");
	if (old_label == NULL)
	{
		/* if old_label is NULL, we have to create a new label */
		hbox = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new (g_strdup_printf ("%s", child->name));
		gtk_widget_show (label);
		//pixmap = gnome_stock_new_with_icon (GNOME_STOCK_MENU_BOOK_OPEN);
		//pixmap = gtk_pixmap_new (pixmap_open, mask_open);
		pixmap = gtk_pixmap_new (pixmap_leaf, mask_leaf);
		gtk_widget_show (pixmap);
		gtk_box_pack_start (GTK_BOX (hbox), pixmap, FALSE, FALSE, 2);
		gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 2);
	}
	else
	{
		hbox = old_label;
		/* old_label is a hbox we created once */
		label = GTK_WIDGET (g_list_next (gtk_container_children (GTK_CONTAINER (old_label)))->data);
		if (label)
			gtk_label_set_text (GTK_LABEL (label), child->name);
	}

	return hbox;
}

GList   *
my_child_create_menus (GnomeMDIChild * child, GtkWidget * view, gpointer data)
{
	GList          *menu_list;
	GtkWidget      *menu, *w;

	g_print ("my_child_create_menus (w = %s) (p = %s) (pp = %s) (ppp = %s) (pppp = %s)\n", gtk_widget_get_name (view), gtk_widget_get_name (view->parent), gtk_widget_get_name (view->parent->parent), gtk_widget_get_name (view->parent->parent->parent), gtk_widget_get_name (view->parent->parent->parent->parent));
	menu_list = NULL;

	/* the Child menu */
	menu = gtk_menu_new ();

	w = gtk_menu_item_new_with_label ("Add View");
	gtk_widget_show (w);

	gtk_menu_append (GTK_MENU (menu), w);
	w = gtk_menu_item_new_with_label ("Remove View");
	gtk_widget_show (w);

	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new_with_label ("Child");
	gtk_widget_show (w);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (w), menu);
	menu_list = g_list_append (menu_list, w);

	return menu_list;
}

void 
app_created (GnomeMDI * mdi, GnomeApp * app)
{
	//g_print ("app_created\n");
	//gnome_app_create_menus (app, main_menu);
	menu_create (mdi, app);
	gtk_window_set_default_size (GTK_WINDOW (app), 500, 500);
	gtk_widget_show_all (GTK_WIDGET (app));
}

void
destroy (GnomeMDI *mdi)
{
	gtk_main_quit();
}

gint
remove_child (GnomeMDI *mdi, GnomeMDIChild *child)
{
	//g_print ("remove child\n");
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

	gnome_mdi_open_toplevel (mdi);

	/* and document menu and document list paths (see gnome-app-helper menu
	   insertion routines for details)  */
	gnome_mdi_set_child_menu_path(mdi, _("File"));
	gnome_mdi_set_child_list_path(mdi, _("Children/"));

	return mdi;
}

void
mdi_add_compare (GnomeMDI *mdi, DiffOptions *diff)
{
	GnomeMDIGenericChild	*child = NULL;
	char			*name  = NULL;

	g_return_if_fail (mdi  != NULL);
	g_return_if_fail (diff != NULL);

	name  = g_strdup_printf ("%s\n%s", diff->left, diff->right);
	child = gnome_mdi_generic_child_new (name);

	g_return_if_fail (child != NULL);
	gnome_mdi_generic_child_set_view_creator (child, my_child_create_compare_view, diff);
	gnome_mdi_generic_child_set_menu_creator (child, my_child_create_menus,      diff);
	gnome_mdi_generic_child_set_label_func   (child, my_child_set_label,         diff);

	gnome_mdi_add_child (mdi, GNOME_MDI_CHILD (child));			/* Add one child to the MDI */
	gnome_mdi_add_view  (mdi, GNOME_MDI_CHILD (child));			/* Display one view of that child */
}

void
mdi_add_diff (GnomeMDI *mdi, DiffOptions *diff, Options *options)	//XXX what about options???
{
	GnomeMDIGenericChild	*child = NULL;
	char			*name  = NULL;

	g_return_if_fail (mdi  != NULL);
	g_return_if_fail (diff != NULL);

	name  = g_strdup_printf ("%s\n%s", diff->left, diff->right);
	child = gnome_mdi_generic_child_new (name);

	g_return_if_fail (child != NULL);
	gnome_mdi_generic_child_set_view_creator (child, my_child_create_view,  diff);
	gnome_mdi_generic_child_set_menu_creator (child, my_child_create_menus, diff);//XXX
	gnome_mdi_generic_child_set_label_func   (child, my_child_set_label,    diff);

	gnome_mdi_add_child (mdi, GNOME_MDI_CHILD (child));			/* Add one child to the MDI */
	gnome_mdi_add_view  (mdi, GNOME_MDI_CHILD (child));			/* Display one view of that child */
}

