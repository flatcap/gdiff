#include <gnome.h>
#include <stdio.h>
#define VERSION "0.0.1"

static gboolean restarted = FALSE;

static void     add_cb (GtkWidget * w);
static void     remove_cb (GtkWidget * w);
static void     add_view_cb (GtkWidget * w);
static void     remove_view_cb (GtkWidget * w);
static void     quit_cb (GtkWidget * w);
static void     mode_top_cb (GtkWidget * w);
static void     mode_book_cb (GtkWidget * w);
static void     mode_modal_cb (GtkWidget * w);

static void     app_created_handler (GnomeMDI *, GnomeApp *);

static gchar   *my_child_get_config_string (GnomeMDIChild *, gpointer);
static GnomeMDIChild *my_child_new_from_config (const gchar *);
static GtkWidget *my_child_set_label (GnomeMDIChild *, GtkWidget *, gpointer);
static GtkWidget *my_child_create_view (GnomeMDIChild *, gpointer);

GnomeMDI       *mdi;

GnomeClient    *client;

/*
 * create_view signal handler: creates any GtkWidget to be used as a view
 * of the child
 */
static GtkWidget *
my_child_create_view (GnomeMDIChild * child, gpointer data)
{
	GtkWidget      *new_view;
	gchar           label[256];

	sprintf (label, "Child %d",
		 GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (child))));

	new_view = gtk_label_new (label);

	return new_view;
}

/*
 * create config string for this child
 */
static gchar   *
my_child_get_config_string (GnomeMDIChild * child, gpointer data)
{
	return g_strdup_printf ("%d", GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (child))));
}

static GtkWidget *
my_child_set_label (GnomeMDIChild * child,
		    GtkWidget * old_label,
		    gpointer data)
{
	GtkWidget      *hbox, *pixmap, *label;
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

/* if we want to use custom-built menus for our child */
static GList   *
my_child_create_menus (GnomeMDIChild * child, GtkWidget * view, gpointer data)
{
	GList          *menu_list;
	GtkWidget      *menu, *w;

	menu_list = NULL;

	/* the Child menu */
	menu = gtk_menu_new ();

	w = gtk_menu_item_new_with_label ("Add View");
	gtk_signal_connect (GTK_OBJECT (w), "activate", GTK_SIGNAL_FUNC (add_view_cb), child);
	gtk_widget_show (w);

	gtk_menu_append (GTK_MENU (menu), w);
	w = gtk_menu_item_new_with_label ("Remove View");
	gtk_signal_connect (GTK_OBJECT (w), "activate", GTK_SIGNAL_FUNC (remove_view_cb), child);
	gtk_widget_show (w);

	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new_with_label ("Child");
	gtk_widget_show (w);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (w), menu);
	menu_list = g_list_append (menu_list, w);

	return menu_list;
}

static GnomeMDIChild *
my_child_new_from_config (const gchar * string)
{
	GnomeMDIGenericChild *child;
	gchar          *name;
	gint            c;

	if (sscanf (string, "%d", &c) != 1)
		return NULL;

	name = g_strdup_printf ("Child %d", c);

	if ((child = gnome_mdi_generic_child_new (name)) != NULL)
	{
		gnome_mdi_generic_child_set_view_creator (child, my_child_create_view, NULL);
		gnome_mdi_generic_child_set_menu_creator (child, my_child_create_menus, NULL);
		gnome_mdi_generic_child_set_config_func (child, my_child_get_config_string, NULL);
		gnome_mdi_generic_child_set_label_func (child, my_child_set_label, NULL);

		gtk_object_set_user_data (GTK_OBJECT (child), GINT_TO_POINTER (c));
	}

	return GNOME_MDI_CHILD (child);
}

static void 
quit_cb (GtkWidget * widget)
{
	/* when the user wants to quit we try to remove all children and if we
	 * succeed destroy the MDI. if TRUE was passed as the second (force)
	 * argument, remove_child signal wouldn't be emmited.
	 */
	if (gnome_mdi_remove_all (mdi, FALSE))
		gtk_object_destroy (GTK_OBJECT (mdi));
}

static void 
cleanup_cb (GnomeMDI * mdi)
{
	/* on destruction of GnomeMDI we call gtk_main_quit(), since we have opened
	 * no windows on our own and therefore our GUI is gone.
	 */
	gtk_main_quit ();
}

static void 
add_view_cb (GtkWidget * w)
{
	/* our child-menu-item activate signal handler also gets the pointer to
	 * the child that this menu item belongs to as the second argument
	 */
	GnomeMDIChild  *child;

	if (mdi->active_view)
	{
		child = gnome_mdi_get_child_from_view (mdi->active_view);
		gnome_mdi_add_view (mdi, child);
	}
}

static void 
remove_view_cb (GtkWidget * w)
{
	/* mdi->active_view holds the pointer to the view that this action
	 * applies to
	 */
	if (mdi->active_view)
		gnome_mdi_remove_view (mdi, mdi->active_view, FALSE);
}

static void 
remove_cb (GtkWidget * w)
{
	/* mdi->active_child holds the pointer to the child that this action
	 * applies to
	 */
	if (mdi->active_view)
		gnome_mdi_remove_child (mdi,
					gnome_mdi_get_child_from_view (mdi->active_view),
					FALSE);
}

static void 
add_cb (GtkWidget * w)
{
	static gint     counter = 1;
	gchar           name[32];
	GnomeMDIGenericChild *child;

	sprintf (name, "Child %d", counter);

	if ((child = gnome_mdi_generic_child_new (name)) != NULL)
	{
		gnome_mdi_generic_child_set_view_creator (child, my_child_create_view, NULL);
		gnome_mdi_generic_child_set_menu_creator (child, my_child_create_menus, NULL);
		gnome_mdi_generic_child_set_config_func (child, my_child_get_config_string, NULL);
		gnome_mdi_generic_child_set_label_func (child, my_child_set_label, NULL);

		gtk_object_set_user_data (GTK_OBJECT (child), GINT_TO_POINTER (counter));

		/* add the child to MDI */
		gnome_mdi_add_child (mdi, GNOME_MDI_CHILD (child));

		/* and add a new view of the child */
		gnome_mdi_add_view (mdi, GNOME_MDI_CHILD (child));

		counter++;
	}
}

static void 
mode_top_cb (GtkWidget * w)
{
	gnome_mdi_set_mode (mdi, GNOME_MDI_TOPLEVEL);
}

static void 
mode_book_cb (GtkWidget * w)
{
	gnome_mdi_set_mode (mdi, GNOME_MDI_NOTEBOOK);
}

static void 
mode_modal_cb (GtkWidget * w)
{
	gnome_mdi_set_mode (mdi, GNOME_MDI_MODAL);
}

static void 
reply_handler (gint reply, gpointer data)
{
	gint           *int_data = (gint *) data;
	*int_data = reply;
	gtk_main_quit ();
}

static gint 
remove_child_handler (GnomeMDI * mdi, GnomeMDIChild * child)
{
	gchar           question[128];
	gint            reply;

	sprintf (question, "Do you really want to remove child %d\n",
		 GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (child))));

	gnome_app_question_modal (gnome_mdi_get_active_window (mdi), question,
				  reply_handler, &reply);

	/* I hope increasing main_level is the proper way to stop an app until
	 * user had replied to this question... */
	gtk_main ();

	if (reply == 0)
		return TRUE;

	return FALSE;
}

static GtkMenuBar *
mdi_create_menus (GnomeMDI * mdi)
{
	GtkWidget      *menu, *w, *bar;

	bar = gtk_menu_bar_new ();

	/* the File menu */
	menu = gtk_menu_new ();

	w = gtk_menu_item_new_with_label ("Add Child");
	gtk_signal_connect (GTK_OBJECT (w), "activate", GTK_SIGNAL_FUNC (add_cb), NULL);
	gtk_widget_show (w);
	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new_with_label ("Remove Child");
	gtk_signal_connect (GTK_OBJECT (w), "activate", GTK_SIGNAL_FUNC (remove_cb), NULL);
	gtk_widget_show (w);
	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new ();
	gtk_widget_show (w);
	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new_with_label ("Exit");
	gtk_signal_connect (GTK_OBJECT (w), "activate", GTK_SIGNAL_FUNC (quit_cb), NULL);
	gtk_widget_show (w);
	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new_with_label ("File");
	gtk_widget_show (w);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (w), menu);

	gtk_menu_bar_append (GTK_MENU_BAR (bar), w);

	menu = gtk_menu_new ();
	w = gtk_menu_item_new_with_label ("Children");
	gtk_widget_show (w);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (w), menu);

	gtk_menu_bar_append (GTK_MENU_BAR (bar), w);

	/* the Mode menu */
	menu = gtk_menu_new ();

	w = gtk_menu_item_new_with_label ("Notebook");
	gtk_signal_connect (GTK_OBJECT (w), "activate", GTK_SIGNAL_FUNC (mode_book_cb), NULL);
	gtk_widget_show (w);
	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new_with_label ("Toplevel");
	gtk_signal_connect (GTK_OBJECT (w), "activate", GTK_SIGNAL_FUNC (mode_top_cb), NULL);
	gtk_widget_show (w);
	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new_with_label ("Modal");
	gtk_signal_connect (GTK_OBJECT (w), "activate", GTK_SIGNAL_FUNC (mode_modal_cb), NULL);
	gtk_widget_show (w);
	gtk_menu_append (GTK_MENU (menu), w);

	w = gtk_menu_item_new_with_label ("MDI Mode");
	gtk_widget_show (w);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (w), menu);

	gtk_menu_bar_append (GTK_MENU_BAR (bar), w);

	return GTK_MENU_BAR (bar);
}

void 
app_created_handler (GnomeMDI * mdi, GnomeApp * app)
{
	gnome_app_set_menus (app, mdi_create_menus (mdi));
}

int 
main (int argc, char **argv)
{

	gboolean        restart_ok = FALSE;

	gnome_init ("gnome-hello-7-mdi", VERSION, argc, argv);

	/* session management init */
	client = gnome_master_client ();

	mdi = GNOME_MDI (gnome_mdi_new ("gnome-hello-7-mdi", "GNOME MDI Hello"));

	/* and document menu and document list paths (see gnome-app-helper menu
	 * insertion routines for details)  */
	gnome_mdi_set_child_menu_path (mdi, "File");
	gnome_mdi_set_child_list_path (mdi, "Children/");

	if (GNOME_CLIENT_CONNECTED (client))
	{
		gnome_config_push_prefix (gnome_client_get_config_prefix (client));

		restarted = gnome_config_get_bool ("General/restarted=0");

		gnome_config_pop_prefix ();
	}
	else
	{
		restarted = FALSE;
	}

	/* connect signals */
	gtk_signal_connect (GTK_OBJECT (mdi), "destroy",
			    GTK_SIGNAL_FUNC (cleanup_cb), NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "remove_child",
			    GTK_SIGNAL_FUNC (remove_child_handler), NULL);
	gtk_signal_connect (GTK_OBJECT (mdi), "app_created",
			    GTK_SIGNAL_FUNC (app_created_handler), NULL);

	/* we could also connect handlers to other signals, but since we're lazy, we won't ;) */

	/* Restore MDI session. */

	if (restarted)
	{
		gnome_config_push_prefix (gnome_client_get_config_prefix (client));

		restart_ok = gnome_mdi_restore_state
			(mdi, "MDI Session", my_child_new_from_config);

		gnome_config_pop_prefix ();
	}
	else
		/* open the initial toplevel window */
		gnome_mdi_open_toplevel (mdi);

	/* and here we go... */
	gtk_main ();

	return 0;
}
		g_print ("client is connected\n");
