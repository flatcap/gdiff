#include <gnome.h>

#define APP "canvas"
#define VER "0.0.1"
#define TTL "Canvas test"

void
destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

gint
item_event (GnomeCanvasItem * item, GdkEvent * event, gpointer data)
{
	static double   x, y;
	double          new_x, new_y;
	GdkCursor      *fleur;
	static int      dragging;
	double          item_x, item_y;
	double          orig_x, orig_y;

	orig_x = item_x = event->button.x;
	orig_y = item_y = event->button.y;
	gnome_canvas_item_w2i (item->parent, &item_x, &item_y);

	//g_print ("event (%d, %d) world (%d, %d)\n", (int) item_x, (int) item_y, (int) orig_x, (int) orig_y);

	switch (event->type)
	{
	case GDK_BUTTON_PRESS:
		switch (event->button.button)
		{
		case 1:
			x = item_x;
			y = item_y;

			fleur = gdk_cursor_new (GDK_FLEUR);
			gnome_canvas_item_grab (item,
						GDK_POINTER_MOTION_MASK |
						GDK_BUTTON_RELEASE_MASK,
						fleur,
						event->button.time);
			gdk_cursor_destroy (fleur);
			dragging = TRUE;
			break;

		default:
			break;
		}
		break;

	case GDK_MOTION_NOTIFY:
		if (dragging && (event->motion.state & GDK_BUTTON1_MASK))
		{
			new_x = item_x;
			new_y = item_y;

			gnome_canvas_item_move (item, new_x - x, new_y - y);
			x = new_x;
			y = new_y;
		}
		break;

	case GDK_BUTTON_RELEASE:
		gnome_canvas_item_ungrab (item, event->button.time);
		dragging = FALSE;
		break;

	default:
		break;
	}

	return FALSE;
}

void size_request (GtkWidget *widget, GtkRequisition *requisition)
{
	//g_print ("request %dx%d\n", requisition->width, requisition->height);
}

void size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
	int    i = MIN (allocation->width, allocation->height);
	double d = i;

	g_print ("alloc   %dx%d at (%d, %d)\n", allocation->width, allocation->height, allocation->x, allocation->y);

	gtk_layout_set_size              (GTK_LAYOUT   (widget), i, i);
	gnome_canvas_set_scroll_region   (GNOME_CANVAS (widget), 0.0, 0.0, 1.0, 1.0);
	gnome_canvas_set_pixels_per_unit (GNOME_CANVAS (widget), d);
}

void
table_canvas (GtkWidget *app)
{
	GtkWidget        *canvas = NULL;
	GnomeCanvasGroup *group  = NULL;
	GnomeCanvasItem  *item   = NULL;
	GtkWidget        *table  = NULL;
	GtkWidget        *label1 = NULL;
	GtkWidget        *label2 = NULL;
	GtkWidget        *scroll = NULL;

	table	= gtk_table_new (1, 2, FALSE);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", (GtkSignalFunc) destroy, NULL);

	gtk_widget_push_visual (gdk_rgb_get_visual ());
	gtk_widget_push_colormap (gdk_rgb_get_cmap ());
	canvas = gnome_canvas_new_aa ();
	gtk_widget_pop_colormap ();
	gtk_widget_pop_visual ();

	gnome_canvas_set_scroll_region (GNOME_CANVAS (canvas), 0.0, 0.0, 100.0, 300.0);
	gtk_layout_set_size (GTK_LAYOUT (canvas), 100, 300);

	group = gnome_canvas_root (GNOME_CANVAS (canvas));

	item = gnome_canvas_item_new (group,
					gnome_canvas_rect_get_type (),
					"x1", 0.0,
					"y1", 2.0,
					"x2", 5.0,
					"y2", 8.0,
					"fill_color_rgba", 0x3cb371FF,
					"outline_color", "pink",
					"width_units", 0.04,
					NULL);
	gtk_signal_connect (GTK_OBJECT (item), "event", (GtkSignalFunc) item_event, NULL);

	gtk_signal_connect (GTK_OBJECT (canvas), "size_request",  (GtkSignalFunc) size_request,  NULL);
	gtk_signal_connect (GTK_OBJECT (canvas), "size_allocate", (GtkSignalFunc) size_allocate, NULL);

	label1 = gtk_label_new ("hello world");
	label2 = gtk_label_new ("goodbye");

	scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scroll), canvas);

	gtk_table_attach (GTK_TABLE (table), label1, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND | GTK_SHRINK, GTK_FILL | GTK_EXPAND | GTK_SHRINK, 10, 10);
	gtk_table_attach (GTK_TABLE (table), scroll, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

	gnome_canvas_set_pixels_per_unit (GNOME_CANVAS (canvas), 10.0);

	gtk_window_set_default_size (GTK_WINDOW (app), 300, 300);
	gnome_app_set_contents (GNOME_APP (app), table);
}

void
scaled_canvas (GtkWidget *app)
{
	GtkWidget        *canvas = NULL;
	GnomeCanvasGroup *group  = NULL;
	GnomeCanvasItem  *item   = NULL;

	gtk_signal_connect (GTK_OBJECT (app), "destroy", (GtkSignalFunc) destroy, NULL);

	gtk_widget_push_visual (gdk_rgb_get_visual ());
	gtk_widget_push_colormap (gdk_rgb_get_cmap ());
	canvas = gnome_canvas_new_aa ();
	gtk_widget_pop_colormap ();
	gtk_widget_pop_visual ();

	gnome_canvas_set_scroll_region (GNOME_CANVAS (canvas), 0.0, 0.0, 1.0, 1.0);
	gtk_layout_set_size (GTK_LAYOUT (canvas), 300, 300);

	group = gnome_canvas_root (GNOME_CANVAS (canvas));

	item = gnome_canvas_item_new (group,
					gnome_canvas_rect_get_type (),
					"x1", 0.01,
					"y1", 0.01,
					"x2", 0.99,
					"y2", 0.99,
					"fill_color_rgba", 0x008000FF,
					//"outline_color", "pink",
					//"width_units", 0.04,
					NULL);
	//gtk_signal_connect (GTK_OBJECT (item), "event", (GtkSignalFunc) item_event, NULL);

	item = gnome_canvas_item_new (group,
					gnome_canvas_rect_get_type (),
					"x1", 0.48,
					"y1", 0.01,
					"x2", 0.52,
					"y2", 0.99,
					"fill_color_rgba", 0x800000FF,
					//"outline_color", "pink",
					//"width_units", 0.04,
					NULL);
	//gtk_signal_connect (GTK_OBJECT (item), "event", (GtkSignalFunc) item_event, NULL);

	item = gnome_canvas_item_new (group,
					gnome_canvas_rect_get_type (),
					"x1", 0.01,
					"y1", 0.48,
					"x2", 0.99,
					"y2", 0.52,
					"fill_color_rgba", 0x000080FF,
					//"outline_color", "pink",
					//"width_units", 0.04,
					NULL);
	//gtk_signal_connect (GTK_OBJECT (item), "event", (GtkSignalFunc) item_event, NULL);

	gtk_signal_connect (GTK_OBJECT (canvas), "size_request",  (GtkSignalFunc) size_request,  NULL);
	gtk_signal_connect (GTK_OBJECT (canvas), "size_allocate", (GtkSignalFunc) size_allocate, NULL);

	gnome_canvas_set_pixels_per_unit (GNOME_CANVAS (canvas), 300.0);

	gtk_window_set_default_size (GTK_WINDOW (app), 300, 300);
	gnome_app_set_contents (GNOME_APP (app), canvas);
}

int
main (int argc, char *argv[])
{
	GtkWidget        *app    = NULL;

	gnome_init (APP, VER, argc, argv);

	app = gnome_app_new (APP, TTL);

	//table_canvas  (app);
	scaled_canvas (app);

	gtk_widget_show_all (app);
	gtk_main();
	return 0;
}

