#include <gnome.h>

static gint     delete_event_cb (GtkWidget * window, GdkEventAny * e, gpointer data);
static void     create_canvas_items (GtkWidget * canvas);

int
main (int argc, char *argv[])
{
	GtkWidget      *window;
	GtkWidget      *sw;
	GtkWidget      *canvas;

	gnome_init ("canvas-example", "0.0", argc, argv);

	gdk_rgb_init ();

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title (GTK_WINDOW (window), "Canvas Example");

	gtk_window_set_policy (GTK_WINDOW (window), TRUE, TRUE, TRUE);

	gtk_signal_connect (GTK_OBJECT (window),
			    "delete_event",
			    GTK_SIGNAL_FUNC (delete_event_cb),
			    NULL);

	sw = gtk_scrolled_window_new (NULL, NULL);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);

	gtk_widget_push_visual (gdk_rgb_get_visual ());
	gtk_widget_push_colormap (gdk_rgb_get_cmap ());
	canvas = gnome_canvas_new_aa ();
	gtk_widget_pop_colormap ();
	gtk_widget_pop_visual ();

	gnome_canvas_set_scroll_region (GNOME_CANVAS (canvas), 0, 0, 600, 450);

	create_canvas_items (canvas);

	gtk_container_add (GTK_CONTAINER (sw), canvas);
	gtk_container_add (GTK_CONTAINER (window), sw);

	gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);

	gtk_widget_show_all (window);

	gtk_main ();

	return 0;
}

static          gint
delete_event_cb (GtkWidget * window, GdkEventAny * e, gpointer data)
{
	gtk_main_quit ();
	return FALSE;
}

static          gint
item_event (GnomeCanvasItem * item, GdkEvent * event, gpointer data)
{
	static double   x, y;
	double          new_x, new_y;
	GdkCursor      *fleur;
	static int      dragging;
	double          item_x, item_y;

	item_x = event->button.x;
	item_y = event->button.y;
	gnome_canvas_item_w2i (item->parent, &item_x, &item_y);

	switch (event->type)
	{
	case GDK_BUTTON_PRESS:
		switch (event->button.button)
		{
		case 1:
			if (event->button.state & GDK_SHIFT_MASK)
			{
				gtk_object_destroy (GTK_OBJECT (item));
			}
			else
			{
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
			}
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

static void
setup_item (GnomeCanvasItem * item)
{
	gtk_signal_connect (GTK_OBJECT (item), "event",
			    (GtkSignalFunc) item_event,
			    NULL);
}

static void
create_canvas_items (GtkWidget * canvas)
{
	GnomeCanvasPoints *points;
	GnomeCanvasGroup *group;
	GnomeCanvasItem *item;
	double          affine[6];

	group = gnome_canvas_root (GNOME_CANVAS (canvas));

	/* A polygon */
	points = gnome_canvas_points_new (14);

	points->coords[0] = 270.0;
	points->coords[1] = 330.0;
	points->coords[2] = 270.0;
	points->coords[3] = 430.0;
	points->coords[4] = 390.0;
	points->coords[5] = 430.0;
	points->coords[6] = 390.0;
	points->coords[7] = 330.0;
	points->coords[8] = 310.0;
	points->coords[9] = 330.0;
	points->coords[10] = 310.0;
	points->coords[11] = 390.0;
	points->coords[12] = 350.0;
	points->coords[13] = 390.0;
	points->coords[14] = 350.0;
	points->coords[15] = 370.0;
	points->coords[16] = 330.0;
	points->coords[17] = 370.0;
	points->coords[18] = 330.0;
	points->coords[19] = 350.0;
	points->coords[20] = 370.0;
	points->coords[21] = 350.0;
	points->coords[22] = 370.0;
	points->coords[23] = 410.0;
	points->coords[24] = 290.0;
	points->coords[25] = 410.0;
	points->coords[26] = 290.0;
	points->coords[27] = 330.0;

	item = gnome_canvas_item_new (group,
				      gnome_canvas_polygon_get_type (),
				      "points", points,
				      "fill_color", "tan",
				      "outline_color", "pink",
				      "width_units", 3.0,
				      NULL);

	setup_item (item);

	gnome_canvas_points_unref (points);

	/* Translate the polygon */

	art_affine_translate (affine, -150.0, -300.0);

	gnome_canvas_item_affine_relative (item, affine);

	/* A translucent rectangle */
	setup_item (gnome_canvas_item_new (group,
					   gnome_canvas_rect_get_type (),
					   "x1", 90.0,
					   "y1", 40.0,
					   "x2", 180.0,
					   "y2", 100.0,
					   "fill_color_rgba", 0x3cb37180,
					   "outline_color", "pink",
					   "width_units", 4.0,
					   NULL));

	/* A translucent ellipse */
	setup_item (gnome_canvas_item_new (group,
					   gnome_canvas_ellipse_get_type (),
					   "x1", 210.0,
					   "y1", 80.0,
					   "x2", 280.0,
					   "y2", 140.0,
					   "fill_color_rgba", 0x5f9ea080,
					   "outline_color", "pink",
					   "width_pixels", 0,
					   NULL));

	/* Create ellipses arranged in a line; they're manipulated as a
	 * single item. */

	group =
		GNOME_CANVAS_GROUP (gnome_canvas_item_new (group,
							   gnome_canvas_group_get_type (),
							   "x", 0.0,
							   "y", 0.0,
							   NULL));
	setup_item (GNOME_CANVAS_ITEM (group));

	{
		double          xpos = 20.0;
		while (xpos < 300.0)
		{
			gnome_canvas_item_new (group,
					       gnome_canvas_ellipse_get_type (),
					       "x1", xpos,
					       "y1", 100.0,
					       "x2", xpos + 10.0,
					       "y2", 110.0,
					       "fill_color_rgba", 0x0000FFFF,
					       "outline_color_rgba", 0xFF,
					       NULL);
			xpos += 15.0;
		}
	}
}
