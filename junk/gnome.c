#include <gnome.h>

#define WINNAME "Test App - hello world"
#define APPNAME "Test App"
#define VERSION "1.2.3"

GtkWidget *app = NULL;

void file_open_cb  (GtkWidget *widget, gpointer data);
void file_close_cb (GtkWidget *widget, gpointer data);
void about_cb      (GtkWidget *widget, gpointer data);
void color_cb      (GtkWidget *widget, gpointer data);

GnomeUIInfo file_menu[] = 
{
	GNOMEUIINFO_MENU_OPEN_ITEM (file_open_cb, NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM(file_close_cb, NULL),
	GNOMEUIINFO_SEPARATOR,
	{GNOME_APP_UI_ITEM, N_("_Change Color"), NULL, color_cb, NULL, NULL, GNOME_APP_PIXMAP_NONE, NULL, GDK_c, GDK_CONTROL_MASK, NULL},
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

void destroy (GtkObject *object, gpointer data)
{
	gtk_exit (0);
}

void color_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *csd = gtk_color_selection_dialog_new ("This is a modal color selection dialog");

	gtk_window_set_modal         (GTK_WINDOW (csd),TRUE);
	gtk_window_set_transient_for (GTK_WINDOW (csd), GTK_WINDOW (app));
	
	gtk_signal_connect_object (GTK_OBJECT (GTK_COLOR_SELECTION_DIALOG(csd)->ok_button),     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT (csd));
	gtk_signal_connect_object (GTK_OBJECT (GTK_COLOR_SELECTION_DIALOG(csd)->cancel_button), "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT (csd));

	gtk_widget_show (csd);    
	gtk_main ();
}

void about_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget *about;
	const char *authors[] = { "jim <jim@ait.co.uk>", "bob <bob@nrma.com.au>", "dave <dave@vmware.com>", NULL };
	const char *copyright = "Copyright 1999";
	const char *extra     = "Extra information";
 
	about = gnome_about_new(APPNAME, VERSION, copyright, authors, extra, NULL);

	gtk_window_set_modal (GTK_WINDOW (about), TRUE);
	gtk_window_set_transient_for (GTK_WINDOW (about), GTK_WINDOW (app));
	
	gtk_widget_show(about);
	gtk_main();
	
	return;
}

void file_open_cb(GtkWidget *widget, gpointer data)
{
	g_print ("file open!\n");
}

void file_close_cb(GtkWidget *widget, gpointer data)
{
	g_print ("file close!\n");
}

void buttons (void)
{
	GtkWidget *frame = gtk_frame_new ("Testing...");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	gnome_app_set_contents (GNOME_APP(app), frame);

	GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);

	gtk_container_add (GTK_CONTAINER (frame), vbox);

	GtkWidget *hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox1), 10);

	GtkWidget *hbox2 = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox2), 10);

	GtkWidget *button1 = gtk_button_new_with_label ("OK");
	GtkWidget *button2 = gtk_button_new_with_label ("Cancel");
	GtkWidget *button3 = gtk_button_new_with_label ("Hello");
	GtkWidget *button4 = gtk_button_new_with_label ("Goodbye");

	gtk_label_parse_uline (GTK_LABEL (GTK_BIN (button3)->child), "H_ello");

	GtkAccelGroup *accel = gtk_accel_group_new ();
	gtk_accel_group_attach (accel, GTK_OBJECT (app));

	gtk_widget_add_accelerator (button1, "clicked", accel, GDK_e, GDK_CONTROL_MASK, (GtkAccelFlags)0);

	gtk_container_add (GTK_CONTAINER (hbox1), button1);
	gtk_container_add (GTK_CONTAINER (hbox1), button2);
	gtk_container_add (GTK_CONTAINER (hbox2), button3);
	gtk_container_add (GTK_CONTAINER (hbox2), button4);

	gtk_container_add (GTK_CONTAINER (vbox), hbox1);
	gtk_container_add (GTK_CONTAINER (vbox), hbox2);

}

void button_press (GtkObject *object, gpointer data)
{
	GtkButton *button = (GtkButton*) data;
	GtkWidget *child  = GTK_BIN (button)->child;
	GtkLabel  *label  = GTK_LABEL (child);

	//char *name = gtk_widget_get_name (GTK_WIDGET (button));

	g_print ("button press '%s'\n", label->label);
}

void checks (void)
{
	GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
	gnome_app_set_contents (GNOME_APP(app), hbox);

	GtkWidget *button1, *button2, *button3, *button4;

	button1 = gtk_check_button_new_with_label ("diff");
	gtk_box_pack_start (GTK_BOX (hbox), button1, TRUE, TRUE, 0);

	button2 = gtk_check_button_new_with_label ("same");
	gtk_box_pack_start (GTK_BOX (hbox), button2, TRUE, TRUE, 0);

	button3 = gtk_check_button_new_with_label ("left");
	gtk_box_pack_start (GTK_BOX (hbox), button3, TRUE, TRUE, 0);

	button4 = gtk_check_button_new_with_label ("right");
	gtk_box_pack_start (GTK_BOX (hbox), button4, TRUE, TRUE, 0);

	gtk_signal_connect_object (GTK_OBJECT (button1), "pressed", GTK_SIGNAL_FUNC (button_press), GTK_OBJECT (button1));
	gtk_signal_connect_object (GTK_OBJECT (button2), "pressed", GTK_SIGNAL_FUNC (button_press), GTK_OBJECT (button2));
	gtk_signal_connect_object (GTK_OBJECT (button3), "pressed", GTK_SIGNAL_FUNC (button_press), GTK_OBJECT (button3));
	gtk_signal_connect_object (GTK_OBJECT (button4), "pressed", GTK_SIGNAL_FUNC (button_press), GTK_OBJECT (button4));
}

void progress (void)
{
	GtkWidget *label = gtk_label_new ("This is the initial label");
	gnome_app_set_contents (GNOME_APP(app), label);

	GdkRectangle rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = 200;
	rect.height = 200;

	gtk_widget_show_all (app);
	gtk_widget_show_now (label);
	gtk_widget_draw (label, &rect);
	gtk_widget_realize (label);
	usleep (5000000);

	gtk_widget_show_now (label);
	gtk_label_set_text (GTK_LABEL (label), "Another label");
	usleep (5000000);

}

void threads (void)
{
#ifdef G_THREADS_ENABLED
	g_print ("G_THREADS_ENABLED\n");
#endif
#ifdef G_THREADS_IMPL_POSIX
	g_print ("G_THREADS_IMPL_POSIX\n");
#endif
#ifdef G_THREADS_IMPL_SOLARIS
	g_print ("G_THREADS_IMPL_SOLARIS\n");
#endif
#ifdef G_THREADS_IMPL_NSPR
	g_print ("G_THREADS_IMPL_NSPR\n");
#endif
#ifdef G_THREADS_IMPL_NONE
	g_print ("G_THREADS_IMPL_NONE\n");
#endif

	g_thread_init (NULL);

	if (g_thread_supported())
	{
		g_print ("threads\n");
	}
	else
	{
		g_print ("no threads\n");
	}
}

void gnome_color (void)
{
	GtkWidget *col = gnome_color_picker_new();
	gnome_app_set_contents (GNOME_APP(app), col);
}

void gnome_rc (void)
{
	char *path   = "/TestRC";
	char *section= "/TestRC/Colors";
	void *handle = NULL;
	char *key    = NULL;
	char *value  = NULL;
	GdkColor col;

	handle = gnome_config_init_iterator_sections (path);
	while (handle)
	{
		key = NULL;
		value = NULL;
		handle = gnome_config_iterator_next (handle, &key, &value);
		if (handle)
			g_print ("[%s], %s\n", key, value);
	}

	handle = gnome_config_init_iterator (section);
	while (handle)
	{
		key = NULL;
		value = NULL;
		handle = gnome_config_iterator_next (handle, &key, &value);
		if (handle)
		{
			if (gdk_color_parse (value, &col))
			{
				g_print ("%s = %.04x %.04x %.04x\n", value, col.red, col.green, col.blue);
			}
		}
	}

}

int main (int argc, char *argv[])
{
	gnome_init (APPNAME, VERSION, argc, argv);

	app = gnome_app_new (APPNAME, WINNAME);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);

	gnome_app_create_menus (GNOME_APP (app), main_menu);

	//buttons();
	//checks();
	//progress();
	//threads();

	gtk_widget_show_all (app);
	gtk_main();

	//gnome_rc();

	return 0;
}

