#include <gnome.h>

#define APP "canvas"
#define VER "0.0.1"
#define TTL "Canvas test"

#define MIN_SIZE	100
#define WIN_HEIGHT	600

enum
{
	same,
	left,
	right,
	diff,

} CompType;

typedef struct
{
	int count;
	int type;
} CompareData;

void draw_rect (double scale);

void
destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

void
size_request (GtkWidget *widget, GtkRequisition *requisition)
{
	//g_print ("request %dx%d\n", requisition->width, requisition->height);
}

GnomeCanvasItem *item = NULL;
GnomeCanvasGroup *group = NULL;

void
size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
	//int    i = MIN (allocation->width, allocation->height);
	//double d = i;
	//int width  = allocation->width;
	int height = allocation->height;
	//int x      = allocation->x;
	//int y      = allocation->y;
	double d;

	//g_print ("pixels / unit %d\n", i);

	//width = MAX (width, 200);

	//XXX g_print ("alloc   %dx%d at (%d, %d)\n", width, height, x, y);

	d = height;
	d /= MIN_SIZE;
	//XXX draw_rect (d);

	//gtk_layout_set_size (GTK_LAYOUT (widget), width, height);
	/*
	if (GTK_WIDGET_REALIZED (widget))
	{
		gdk_window_move_resize (widget->window, x, y, width, height);
	}
	*/

	//gnome_canvas_set_pixels_per_unit (GNOME_CANVAS (widget), d);

	//width  /= 10;
	//height /= 10;
	//gnome_canvas_set_scroll_region   (GNOME_CANVAS (widget), 0.0, 0.0, width, height);
}

void
table_canvas (GtkWidget *app)
{
	GtkWidget        *canvas = NULL;
	GtkWidget        *hbox   = NULL;
	double d;

	hbox = gtk_hbox_new (FALSE, GNOME_PAD_SMALL);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", (GtkSignalFunc) destroy, NULL);

	gtk_widget_push_visual (gdk_rgb_get_visual ());
	gtk_widget_push_colormap (gdk_rgb_get_cmap ());
	canvas = gnome_canvas_new_aa ();
	gtk_widget_pop_colormap ();
	gtk_widget_pop_visual ();

	//gnome_canvas_set_scroll_region (GNOME_CANVAS (canvas), 0.0, 0.0, 200.0, 600.0);

	group = gnome_canvas_root (GNOME_CANVAS (canvas));

	d= WIN_HEIGHT;
	d/=MIN_SIZE;
	draw_rect (d);

	gtk_signal_connect (GTK_OBJECT (canvas), "size_request",  size_request,  NULL);
	gtk_signal_connect (GTK_OBJECT (canvas), "size_allocate", size_allocate, NULL);

	gtk_widget_set_usize (GTK_WIDGET (canvas), MIN_SIZE, MIN_SIZE);

	//gtk_box_pack_start (GTK_BOX (hbox), gtk_entry_new(), TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), canvas, FALSE, FALSE, 0);
	//gtk_box_pack_start (GTK_BOX (hbox), gtk_entry_new(), TRUE, TRUE, 0);

	gnome_canvas_set_pixels_per_unit (GNOME_CANVAS (canvas), MIN_SIZE);

	gtk_window_set_default_size (GTK_WINDOW (app), MIN_SIZE, WIN_HEIGHT);
	gnome_app_set_contents (GNOME_APP (app), hbox);
}

int
main (int argc, char *argv[])
{
	GtkWidget        *app    = NULL;

	gnome_init (APP, VER, argc, argv);

	app = gnome_app_new (APP, TTL);
	table_canvas  (app);

	gtk_widget_show_all (app);
	gtk_main();
	return 0;
}


CompareData sample[] =
{
#if 0
	{ 22,	same	},	// 22
	{ 10,	left	},
	{ 10,	right	},	// 32
	{ 42,	same	},	// 74
	{ 14,	left	},
	{ 24,	right	},	// 98
	{ 32,	same	},	// 130
	{ 66,	left	},
	{  6,	right	},	// 156
	{ 32,	same	},	// 188
#else
	{22,same},{1,left},{1,right},{42,same},{1,left},{2,right},{12,same},{2,left},{2,right},{2,same},
	{1,left},{1,right},{15,same},{1,left},{1,right},{1,same},{1,left},{1,right},{6,same},{1,left},
	{2,right},{8,same},{1,left},{1,right},{27,same},{1,right},{14,same},{1,left},{7,right},{9,same},
	{1,left},{1,same},{1,left},{1,right},{14,same},{2,right},{1,same},{1,left},{7,same},{2,left},
	{1,right},{6,same},{1,left},{1,right},{1,same},{1,left},{2,right},{1,same},{2,right},{1,same},
	{2,left},{1,same},{12,left},{1,right},{1,same},{4,left},{8,right},{1,same},{4,right},{3,same},
	{12,left},{1,right},{1,same},{4,left},{1,right},{1,same},{9,left},{7,right},{23,same},{1,right},
	{75,same},{1,left},{2,same},{1,left},{1,right},{3,same},{1,right},{1,same},{1,right},{4,same},
	{1,left},{2,same},{1,left},{1,right},{3,same},{1,left},{3,same},{1,left},{1,right},{14,same},
	{1,right},{24,same},{1,left},{1,right},{2,same},{2,right},{2,same},{1,left},{10,same},{6,right},
	{26,same},{1,left},{1,right},{17,same},{2,left},{2,right},{2,same},{1,left},{1,right},{7,same},
	{4,left},{4,right},{22,same},{1,right},{19,same},{1,left},{1,right},{5,same},{1,right},{5,same},
	{1,left},{1,right},{11,same},{1,left},{2,right},{56,same},{15,right},{2,same},{1,left},{2,right},
	{3,same},{1,left},{24,same},{1,left},{1,right},{20,same},{4,left},{9,right},{1,same},{1,left},
	{1,right},{1,same},{7,left},{3,right},{6,same},{8,left},{3,right},{5,same},{2,left},{3,right},
	{7,same},{1,left},{1,right},{3,same},{1,left},{1,right},{1,same},{3,left},{3,right},{5,same},
	{2,left},{2,right},{2,same},{3,left},{4,right},{1,same},{2,left},{1,right},{3,same},{2,left},
	{2,right},{9,same},{1,left},{1,right},{2,same},{3,left},{1,right},{1,same},{2,left},{1,same},
	{1,left},{1,right},{1,same},{3,left},{1,right},{6,same},{3,left},{4,right},{1,same},{4,left},
	{4,right},{2,same},{4,left},{1,right},{1,same},{1,left},{1,right},{1,same},{5,left},{1,right},
	{2,same},{1,left},{1,right},{1,same},{1,left},{2,right},{2,same},{13,left},{3,right},{1,same},
	{1,left},{1,right},{1,same},{8,left},{2,right},{2,same},{2,left},{3,right},{1,same},{8,left},
	{8,right},{1,same},{5,left},{5,right},{30,same},{19,right},{7,same},{1,left},{1,right},{8,same},
	{2,right},{2,same},{1,left},{2,same},{1,left},{6,same},{1,left},{1,right},{5,same},{9,left},
	{4,right},{3,same},{1,left},{9,right},{2,same},{1,left},{3,right},{1,same},{4,left},{4,right},
	{2,same},{1,left},{4,same},{1,left},{1,right},{9,same},{2,left},{3,right},{4,same},{2,left},
	{2,same},{1,left},{1,right},
#endif
	{  0 }
};

// affine transformation -- draw at (0,0), then translate to (0.1, 0.1)
// so as to avoid scaling the border
// gtk_layout_freeze / thaw
// group all the items
// hopefully we can scale the whole group in one go

void
draw_rect (double scale)
{
	const double epsilon		= 0.1;
	const double lx1		= 0.35;
	const double lx2		= 0.45;
	const double rx1		= 0.55;
	const double rx2		= 0.65;
	const gulong same_colour	= 0xffffffff;
	const gulong left_colour	= 0xff0000ff;
	const gulong right_colour	= 0xdfdf00ff;

	double ly1 = epsilon;
	double ly2 = epsilon;
	double ry1 = epsilon;
	double ry2 = epsilon;

	double lsize = 0.0;
	double rsize = 0.0;

	GtkLayout *layout = NULL;
	gulong colour;
	double unit;
	double size;
	int type;
	int i;

	layout = GTK_LAYOUT (GNOME_CANVAS_ITEM (group)->canvas);
	gtk_layout_freeze (layout);

	for (i = 0; sample[i].count; i++)
	{
		if (sample[i].type == same)
		{
			lsize += sample[i].count;
			rsize += sample[i].count;
		}
		else if (sample[i].type == left)
		{
			lsize += sample[i].count;
		}
		else // right
		{
			rsize += sample[i].count;
		}
	}

	unit = (scale - epsilon*2) / (MAX (lsize, rsize));

	for (i = 0; sample[i].count; i++)
	{
		size = sample[i].count * unit;
		type = sample[i].type;

		if (type == left)
		{
			colour = left_colour;
		}
		else if (type == right)
		{
			colour = right_colour;
		}
		else
		{
			colour = same_colour;
		}

		if ((type == same) || (type == left))
		{
			ly2 += size;
			item = gnome_canvas_item_new (group, gnome_canvas_rect_get_type(),
				"x1", lx1, "y1", ly1, "x2", lx2, "y2", ly2, "fill_color_rgba", colour, NULL);
		}
		if ((type == same) || (type == right))
		{
			ry2 += size;
			item = gnome_canvas_item_new (group, gnome_canvas_rect_get_type(),
				"x1", rx1, "y1", ry1, "x2", rx2, "y2", ry2, "fill_color_rgba", colour, NULL);
		}

		ly1 = ly2;
		ry1 = ry2;
	}

	gtk_layout_thaw (layout);
}

