#include <gnome.h>

#define APP "window"
#define VER "0.0.1"
#define TTL "Window!"

void menu_cb (GtkWidget * widget, gpointer data) {}

GnomeUIInfo windows_menu[] = {
	GNOMEUIINFO_END
};

GnomeUIInfo fruit_menu[] = {
	GNOMEUIINFO_ITEM("Apple",  "tooltip1", menu_cb, NULL),//NULL=pixmap
	GNOMEUIINFO_ITEM("Banana", "tooltip2", menu_cb, NULL),
	GNOMEUIINFO_ITEM("Cherry", "tooltip3", menu_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo     file_menu[] =
{
	GNOMEUIINFO_MENU_OPEN_ITEM  (menu_cb, NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM (menu_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo     main_menu[] =
{
	GNOMEUIINFO_MENU_FILE_TREE (file_menu),
	GNOMEUIINFO_SUBTREE("_Windows", windows_menu),
	GNOMEUIINFO_SUBTREE("_Fruit", fruit_menu),
	GNOMEUIINFO_END
};

void
destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

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
app_created (GnomeMDI * mdi, GnomeApp * app)
{
	g_print ("app_created\n");
	gnome_app_create_menus (app, main_menu);
	gtk_window_set_default_size (GTK_WINDOW (app), 500, 500);
}

void
add_window (GnomeMDI *mdi)
{
	static gint counter = 1;
	gchar       name[32];
	GnomeMDIGenericChild *child;

	sprintf (name, "Child %d, much longer\nline 2!", counter);

	child = gnome_mdi_generic_child_new (name);

	gnome_mdi_generic_child_set_view_creator (child, my_child_create_view,       NULL);
	gnome_mdi_generic_child_set_menu_creator (child, my_child_create_menus,      NULL);
	gnome_mdi_generic_child_set_config_func  (child, my_child_get_config_string, NULL);
	gnome_mdi_generic_child_set_label_func   (child, my_child_set_label,         NULL);

	gtk_object_set_user_data (GTK_OBJECT (child), GINT_TO_POINTER (counter));

	gnome_mdi_add_child (mdi, GNOME_MDI_CHILD (child)); /* add the child to MDI */
	gnome_mdi_add_view  (mdi, GNOME_MDI_CHILD (child)); /* and add a new view of the child */

	counter++;
}

int
main (int argc, char *argv[])
{
	GnomeClient *client = NULL;
	GnomeMDI    *mdi    = NULL;

	gnome_init (APP, VER, argc, argv);

	client = gnome_master_client();

	g_print ("client id = '%s'\n", client->client_id);

	mdi = GNOME_MDI (gnome_mdi_new (APP, TTL));

	if (GNOME_CLIENT_CONNECTED (client))
	{
		g_print ("client is connected\n");
	}
	else
	{
		g_print ("client is disconnected\n");
	}

	gtk_signal_connect (GTK_OBJECT (mdi), "destroy",     GTK_SIGNAL_FUNC (destroy),     NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "app_created", GTK_SIGNAL_FUNC (app_created), NULL);

#if 0
	gint (*add_child)	(GnomeMDI *, GnomeMDIChild *); 
	gint (*remove_child)	(GnomeMDI *, GnomeMDIChild *); 
	gint (*add_view)	(GnomeMDI *, GtkWidget *); 
	gint (*remove_view)	(GnomeMDI *, GtkWidget *); 
	void (*child_changed)	(GnomeMDI *, GnomeMDIChild *);
	void (*view_changed)	(GnomeMDI *, GtkWidget *);
	void (*app_created)	(GnomeMDI *, GnomeApp *);
#endif

	mdi->tab_pos = GTK_POS_TOP;		// GTK_POS_LEFT, GTK_POS_RIGHT, GTK_POS_TOP, GTK_POS_BOTTOM

	gnome_mdi_open_toplevel (mdi);
	gnome_mdi_set_mode      (mdi, GNOME_MDI_NOTEBOOK); // GNOME_MDI_NOTEBOOK GNOME_MDI_TOPLEVEL GNOME_MDI_MODAL

	//gnome_mdi_set_child_menu_path (mdi, "_File");		// Place after this menu item
	gnome_mdi_set_child_list_path (mdi, "_Windows/");	// ditto (can be in sub-(sub...)item
								// If you want them

	add_window (mdi);
	add_window (mdi);
	add_window (mdi);

	g_print ("appname = %s\n", mdi->appname);
	g_print ("title   = %s\n", mdi->title);

	{
		GList *l = mdi->windows;
		GnomeApp *a = NULL;
		GtkNotebook *nb = NULL;
		while (l)
		{
			a = GNOME_APP (l->data);
			g_print ("app = %s\n", a->name);
			g_print ("app contents = %s\n", gtk_widget_get_name (a->contents));
			nb = GTK_NOTEBOOK (a->contents);

			g_print ("notebook menu  = %p\n", (nb->menu));
			g_print ("notebook panel = %p\n", (nb->panel));
			{
				GList *l3 = (GTK_NOTEBOOK (a->contents))->children;
				GtkNotebookPage *p = NULL;
				while (l3)
				{
					p = l3->data;
					g_print ("nchild = %p\n", p);
					g_print ("menu = %p\n", (p->menu_label));
					g_print ("tab  = %s\n", gtk_widget_get_name (p->tab_label));
					{
						GtkBox *b = GTK_BOX (p->tab_label);
						GList *l4 = (b->children);
						while (l4)
						{
							g_print ("bchild = %p\n", l4->data);
							
							l4 = l4->next;
						}
					}
					
					l3 = l3->next;
				}
			}
			
			l = l->next;
		}
	}

	{
		GList *l = mdi->children;
		GnomeMDIChild *c = NULL;
		while (l)
		{
			c = GNOME_MDI_CHILD (l->data);
			g_print ("child = %s\n", c->name);
			{
				GList *l2 = c->views;
				GtkWidget *w = NULL;
				while (l2)
				{
					w = GTK_WIDGET (l2->data);
					g_print ("view = %s\n", gtk_widget_get_name (GTK_WIDGET (w)));
					
					l2 = l2->next;
				}
			}

			
			l = l->next;
		}
	}

	gtk_main();
	return 0;
}

