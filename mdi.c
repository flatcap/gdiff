#include <gnome.h>
#include "options.h"
#include "mdi.h"
#include "menu.h"

GtkWidget *
my_child_create_view (GnomeMDIChild * child, gpointer data)
{
	//GtkWidget      *new_view;
	gchar           label[256];
	GtkWidget      *scroll = NULL;

	g_print ("my_child_create_view\n");
	sprintf (label, "Child %d",
		 GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (child))));

	scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (scroll), gtk_label_new (label));

	return scroll;
}

gchar   *
my_child_get_config_string (GnomeMDIChild * child, gpointer data)
{
	g_print ("my_child_get_config_string\n");
	return g_strdup_printf ("%d", GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (child))));
}

GtkWidget *
my_child_set_label (GnomeMDIChild * child,
		    GtkWidget * old_label,
		    gpointer data)
{
	GtkWidget      *hbox, *pixmap, *label;
	g_print ("my_child_set_label\n");
	if (old_label == NULL)
	{
		/* if old_label is NULL, we have to create a new label */
		hbox = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new (child->name);
		gtk_widget_show (label);
		pixmap = gnome_stock_new_with_icon (GNOME_STOCK_MENU_TRASH_FULL);
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

	g_print ("my_child_create_menus\n");
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
mdi_add_dummy_window (GnomeMDI *mdi)
{
	// is it possible to show / hide a dummy window?
	// I want an MDI with nothing in it
	//GnomeMDIGenericChild *child = gnome_mdi_generic_child_new ("dummy");

	//gnome_mdi_generic_child_set_view_creator (child, my_child_create_view,       NULL);
	//gnome_mdi_generic_child_set_menu_creator (child, my_child_create_menus,      NULL);
	//gnome_mdi_generic_child_set_config_func  (child, my_child_get_config_string, NULL);
	//gnome_mdi_generic_child_set_label_func   (child, my_child_set_label,         NULL);

	//gnome_mdi_add_child (mdi, GNOME_MDI_CHILD (child));
	//gnome_mdi_add_view  (mdi, GNOME_MDI_CHILD (child));
}

void
mdi_remove_dummy_window (GnomeMDI *mdi)
{
}

void 
app_created (GnomeMDI * mdi, GnomeApp * app)
{
	//g_print ("app_created\n");
	//gnome_app_create_menus (app, main_menu);
	menu_create (app);
	gtk_window_set_default_size (GTK_WINDOW (app), 500, 500);
}

GnomeMDI *
mdi_new (gchar *appname, gchar *title)
{
	GnomeMDI *mdi = NULL;

	mdi = GNOME_MDI (gnome_mdi_new (appname, title));

	mdi->tab_pos = GTK_POS_TOP;		// GTK_POS_LEFT, GTK_POS_RIGHT, GTK_POS_TOP, GTK_POS_BOTTOM
	gnome_mdi_set_mode (mdi, GNOME_MDI_MODAL); // GNOME_MDI_NOTEBOOK GNOME_MDI_TOPLEVEL GNOME_MDI_MODAL

	gtk_signal_connect (GTK_OBJECT (mdi), "app_created", GTK_SIGNAL_FUNC (app_created), NULL);

	mdi_add_dummy_window (mdi);
	gnome_mdi_open_toplevel (mdi);

	return mdi;
}

void
mdi_add_diff (GnomeMDI *mdi, Options *options, DiffOptions *diff)
{
	static gint counter = 1;
	gchar       name[32];
	GnomeMDIGenericChild *child;

	sprintf (name, "Child %d, much longer\nline 2!", counter);

	gnome_mdi_set_mode (mdi, GNOME_MDI_MODAL); // GNOME_MDI_NOTEBOOK GNOME_MDI_TOPLEVEL GNOME_MDI_MODAL
	child = gnome_mdi_generic_child_new (name);

	gnome_mdi_generic_child_set_view_creator (child, my_child_create_view,       NULL);
	gnome_mdi_generic_child_set_menu_creator (child, my_child_create_menus,      NULL);
	gnome_mdi_generic_child_set_config_func  (child, my_child_get_config_string, NULL);
	gnome_mdi_generic_child_set_label_func   (child, my_child_set_label,         NULL);

	gtk_object_set_user_data (GTK_OBJECT (child), GINT_TO_POINTER (counter));

	//gnome_mdi_add_child (mdi, GNOME_MDI_CHILD (child)); /* add the child to MDI */
	gnome_mdi_add_view  (mdi, GNOME_MDI_CHILD (child)); /* and add a new view of the child */

	counter++;

	mdi_remove_dummy_window (mdi);
}

void
mdi_show_all (GnomeMDI *mdi)
{
	GList    *list = NULL;
	GnomeApp *app  = NULL;

	list = mdi->windows;

	while (list)
	{
		app = GNOME_APP (list->data);

		gtk_widget_show_all (GTK_WIDGET (app));

		list = list->next;
	}
}

