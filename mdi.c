/*
    mdi.c -- description
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

/* $Revision: 1.35 $ */

#include <gnome.h>
#include "mdi.h"
#include "menu.h"
#include "derived.h"
#include "compare.h"
#include "global.h"

/*----------------------------------------------------------------------------*/
static GtkWidget *	gd_mdi_set_label		(GnomeMDIChild *child, GtkWidget *old_label, gpointer data);
static GtkWidget *	gd_mdi_create_view		(GnomeMDIChild *child, gpointer data);
static GtkWidget *	gd_mdi_create_file_view		(MDIDiffChild *child);
static GtkWidget *	gd_mdi_create_list_view		(MDIDiffChild *child);
static gint		remove_child			(GnomeMDI *mdi, GnomeMDIChild *child);
static void		app_created			(GnomeMDI *mdi, GnomeApp *app);
static void		destroy				(GnomeMDI *mdi);

static void		gtk_mdi_diff_child_class_init	(MDIDiffChildClass *klass);
static void		gtk_mdi_diff_child_init		(MDIDiffChild *child);

GnomeMDI *	mdi_new (gchar *appname, gchar *title);
void		mdi_add_diff (GnomeMDI *mdi, DiffOptions *diff);
GtkWidget *	mdi_get_current_view (GnomeMDI *mdi);
void		mdi_close (GnomeMDI *mdi);
/*----------------------------------------------------------------------------*/

#if 0
	gint        (*add_child)(GnomeMDI *, GnomeMDIChild *); 
	gint        (*remove_child)(GnomeMDI *, GnomeMDIChild *); 
	gint        (*add_view)(GnomeMDI *, GtkWidget *); 
	gint        (*remove_view)(GnomeMDI *, GtkWidget *); 
	void        (*child_changed)(GnomeMDI *, GnomeMDIChild *);
	void        (*view_changed)(GnomeMDI *, GtkWidget *);
	void        (*app_created)(GnomeMDI *, GnomeApp *);
#endif

GtkWidget *
mdi_get_current_view (GnomeMDI *mdi)
{
	GtkWidget *widget = NULL;
	GtkBin    *bin    = NULL;

	g_return_val_if_fail (mdi != NULL, NULL);
	widget = gnome_mdi_get_active_view (mdi);

	g_return_val_if_fail (widget != NULL, NULL);
	bin = GTK_BIN (widget);					// GtkScrolledWindow

	g_return_val_if_fail (bin != NULL, NULL);
	return bin->child;
}

static void
view_changed (GnomeMDI *mdi, GtkWidget *oldview)
{
	GtkBin *bin = NULL;

	if (oldview)
	{
		bin = GTK_BIN (oldview);
		//g_print ("old view = %s\n", gtk_widget_get_name (bin->child));
	}
	else
	{
		//g_print ("old view = NULL\n");
	}

	bin = GTK_BIN (mdi->active_view);
	//g_print ("new view = %s\n", gtk_widget_get_name (bin->child));
	//g_print ("object = %d\n", gtk_object_get_type());
	//g_print ("view   = %d\n", GTK_OBJECT_TYPE (bin->child));

	//g_print ("active_child = %p\nactive_view = %p\nactive_window = %p\n\n", mdi->active_child, mdi->active_view, mdi->active_window);

	// change the menus (obj type)
	set_menu_state (mdi);
}

static GtkWidget *
gd_mdi_create_view (GnomeMDIChild *child, gpointer data)
{
	MDIDiffChild *dchild = GTK_MDI_DIFF_CHILD (child);

	if ((dchild->diff_options->type == Dir) || (dchild->diff_options->type == DirPatch))
	{
		return gd_mdi_create_list_view (dchild);
	}
	else
	{
		return gd_mdi_create_file_view (dchild);
	}

}

static GtkWidget *
gd_mdi_create_file_view (MDIDiffChild *child)
{
	GtkWidget   *compare = gtk_compare_new (child->diff_options);
	GtkWidget   *scroll  = gtk_scrolled_window_new (NULL, NULL);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (scroll), compare);

	gtk_widget_show_all (scroll);
	return scroll;
}

static GtkWidget *
gd_mdi_create_list_view (MDIDiffChild *child)
{
	GtkWidget      *scroll = NULL;
	GtkWidget      *tree   = NULL;

	g_return_val_if_fail (child != NULL, NULL);

	scroll = gtk_scrolled_window_new (NULL, NULL);

	g_return_val_if_fail (scroll != NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

	tree = gtk_diff_tree_new (2, 0, child->diff_options);

	gtk_container_add (GTK_CONTAINER (scroll), tree);

	gtk_widget_show_all (scroll);
	return scroll;
}

static GtkWidget *
gd_mdi_set_label (GnomeMDIChild *child, GtkWidget *old_label, gpointer data)
{
	// This label will show up in both the notebook tab AND the window menu
	GtkWidget	*hbox   = NULL;
	GtkWidget	*pixmap = NULL;
	GtkWidget	*label  = NULL;
	DiffOptions	*diff   = GTK_MDI_DIFF_CHILD (child)->diff_options;

	if (old_label == NULL)						/* We have to create a new label */
	{
		hbox = gtk_hbox_new (FALSE, 0);
		label = gtk_label_new (g_strdup_printf ("%s", child->name));
		gtk_widget_show (label);

		if ((diff->type == Dir) || (diff->type == DirPatch))
		{
			pixmap = gtk_pixmap_new (pixmap_open, mask_open);
		}
		else
		{
			pixmap = gtk_pixmap_new (pixmap_leaf, mask_leaf);
		}

		gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);

		gtk_widget_show (pixmap);
		gtk_box_pack_start (GTK_BOX (hbox), pixmap, FALSE, FALSE, 2);
		gtk_box_pack_start (GTK_BOX (hbox), label,  FALSE, FALSE, 2);
	}
	else
	{
		hbox = old_label;
	}

	return hbox;
}

static void 
app_created (GnomeMDI *mdi, GnomeApp *app)
{
	menu_create (mdi, app);
	gtk_window_set_default_size (GTK_WINDOW (app), 500, 500);
	gtk_widget_show_all (GTK_WIDGET (app));
}

static void
destroy (GnomeMDI *mdi)
{
	gtk_main_quit();
}

static void
child_destroy (GtkObject *object)
{
	GtkObjectClass *parent = NULL;

	//XXX free the diff_options
	// set all struct members to NULL

	parent = gtk_type_class (gnome_mdi_child_get_type());		// my parent class

	g_return_if_fail (parent != NULL);

	parent->destroy (object);
}

static gint
remove_child (GnomeMDI *mdi, GnomeMDIChild *child)
{
	//XXX ask config for confirm, if compare just close, if tree 'close all compares, too?'
	g_print ("remove child\n");
	return TRUE;			// yes let it die
}

void
mdi_close_view (GnomeMDI *mdi, GnomeMDIChild *child)
{
	gnome_mdi_remove_child (mdi, child, FALSE);
	set_menu_state (mdi);
}

void
mdi_close (GnomeMDI *mdi)
{
	g_print ("mdi_close\n");
	gnome_mdi_remove_all (mdi, FALSE);
	gtk_main_quit();
}

GnomeMDI *
mdi_new (gchar *appname, gchar *title)
{
	GnomeMDI *mdi = NULL;

	mdi = GNOME_MDI (gnome_mdi_new (appname, title));

	menu_create_main (mdi);

	mdi->tab_pos = GTK_POS_TOP;		// GTK_POS_LEFT, GTK_POS_RIGHT, GTK_POS_TOP, GTK_POS_BOTTOM
	gnome_mdi_set_mode (mdi, GNOME_MDI_NOTEBOOK); // GNOME_MDI_NOTEBOOK GNOME_MDI_TOPLEVEL GNOME_MDI_MODAL

	gtk_signal_connect (GTK_OBJECT (mdi), "app_created",  GTK_SIGNAL_FUNC (app_created),  NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "destroy",      GTK_SIGNAL_FUNC (destroy),      NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "remove_child", GTK_SIGNAL_FUNC (remove_child), NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "view_changed", GTK_SIGNAL_FUNC (view_changed), NULL);

	gnome_mdi_open_toplevel (mdi);
	set_menu_state (mdi);

	return mdi;
}

void
mdi_add_diff (GnomeMDI *mdi, DiffOptions *diff)
{
	MDIDiffChild	*child = NULL;
	char		*name  = NULL;
	GtkType		 type  = 0;

	g_return_if_fail (mdi  != NULL);
	g_return_if_fail (diff != NULL);

	if (diff->relative)
	{
		name  = g_strdup_printf ("%s\n%s\n%s", diff->left_root, diff->right_root, diff->relative);
	}
	else
	{
		name  = g_strdup_printf ("%s\n%s", diff->left, diff->right);
	}

	if ((diff->type == Dir) || (diff->type == DirPatch))
	{
		type = gtk_diff_tree_get_type();
	}
	else
	{
		type = gtk_compare_get_type();
	}

	child = gtk_mdi_diff_child_new (name, diff);
	g_return_if_fail (child != NULL);

	set_menu_for_view (child);

	gnome_mdi_add_child (mdi, GNOME_MDI_CHILD (child));			/* Add one child to the MDI */
	gnome_mdi_add_view  (mdi, GNOME_MDI_CHILD (child));			/* Display one view of that child */
	//g_print ("mdi_add_diff %p %p\n", mdi, GNOME_MDI_CHILD (child)->parent);
}

/*______________________________________________________________________________
*/

guint
gtk_mdi_diff_child_get_type (void)
{
	static guint mdi_diff_child_type = 0;
	static const GtkTypeInfo mdi_diff_child_info =
	{
		"MDIDiffChild",
		sizeof (MDIDiffChild),
		sizeof (MDIDiffChildClass),
		(GtkClassInitFunc)  gtk_mdi_diff_child_class_init,
		(GtkObjectInitFunc) gtk_mdi_diff_child_init,
		/* reserved */ NULL,
		/* reserved */ NULL,
		(GtkClassInitFunc) NULL,
	};

	//g_print ("gtk_mdi_diff_child_get_type\n");
	if (!mdi_diff_child_type)
	{
		// has to be 'derived' from GnomeMDIChild to be able to cast to it!
		mdi_diff_child_type = gtk_type_unique (gnome_mdi_child_get_type(), &mdi_diff_child_info);
	}

	return mdi_diff_child_type;
}

MDIDiffChild *
gtk_mdi_diff_child_new (char *name, DiffOptions *diff)
{
	MDIDiffChild *child = NULL;

	child = gtk_type_new (gtk_mdi_diff_child_get_type());

	g_return_val_if_fail (child != NULL, NULL);

	child->diff_options = diff;
	GNOME_MDI_CHILD (child)->name = g_strdup (name);

	return child;
}

static void
gtk_mdi_diff_child_class_init (MDIDiffChildClass *klass)
{
	GnomeMDIChildClass *gnome  = (GnomeMDIChildClass*) klass;
	GtkObjectClass     *object = (GtkObjectClass*)     klass;

	gnome->set_label	 = gd_mdi_set_label;
	gnome->create_view	 = gd_mdi_create_view;
	gnome->create_menus	 = NULL;
	gnome->get_config_string = NULL;

	object->destroy		 =  child_destroy;
}

static void
gtk_mdi_diff_child_init (MDIDiffChild *child)
{
	//XXX not ACTUALLY necessary
	child->diff_options = NULL;
}

