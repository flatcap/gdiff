#include <gnome.h>

#define APP "canvas"
#define VER "0.0.1"
#define TTL "Canvas test"

#define MIN_WIDTH	 50
#define MIN_HEIGHT	100

#define WIN_WIDTH	 50
#define WIN_HEIGHT	800

const double x_indent        =  18.0;
const double y_indent        =  10.0;
const double bar_width       =   6.0;
const double min_height      =  50.0;
const double window_width    =  50.0;
const double view_line_width =   3.0;

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

void draw_rect (double scale, GnomeCanvasGroup *group);


void
destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

//GnomeCanvasItem *item = NULL;
//GnomeCanvasGroup *group = NULL;

void
table_canvas (GtkWidget *app)
{
	GtkWidget        *canvas = NULL;
	GtkWidget        *hbox   = NULL;
	double d;
	GnomeCanvasGroup *group = NULL;

	hbox = gtk_hbox_new (FALSE, GNOME_PAD_SMALL);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", (GtkSignalFunc) destroy, NULL);

	gtk_widget_push_visual (gdk_rgb_get_visual());
	gtk_widget_push_colormap (gdk_rgb_get_cmap());
	canvas = gnome_canvas_new_aa();
	gtk_widget_pop_colormap();
	gtk_widget_pop_visual();

	group = gnome_canvas_root (GNOME_CANVAS (canvas));

	d= WIN_HEIGHT;
	//d/=50.0;
	draw_rect (d, group);

	gtk_widget_set_usize (GTK_WIDGET (canvas), MIN_WIDTH, MIN_HEIGHT);

	//gtk_box_pack_start (GTK_BOX (hbox), gtk_entry_new(), TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), canvas, FALSE, FALSE, 0);
	//gtk_box_pack_start (GTK_BOX (hbox), gtk_entry_new(), TRUE, TRUE, 0);

	gtk_window_set_default_size (GTK_WINDOW (app), WIN_WIDTH, WIN_HEIGHT);

	gnome_canvas_set_pixels_per_unit (GNOME_CANVAS (canvas), 1.0);

	// I'm not quite sure what's going on here, but if we make the scrolling region huge,
	// then the canvas won't keep trying to centre everything we draw
	gnome_canvas_set_scroll_region   (GNOME_CANVAS (canvas), 0.0, 0.0, WIN_WIDTH*10, WIN_HEIGHT*10);

	gnome_app_set_contents (GNOME_APP (app), hbox);
}

int
main (int argc, char *argv[])
{
	GtkWidget        *app    = NULL;

	gnome_init (APP, VER, argc, argv);

	app = gnome_app_new (APP, TTL);
	table_canvas (app);

	gtk_widget_show_all (app);
	gtk_main();
	return 0;
}


CompareData sample[] =
{
#if 1
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
	{22,same},{1,left},{1,right},{42,same},{1,left},{2,right},{12,same},{2,left},{2,right},{2,same},{1,left},{1,right},{15,same},{1,left},{1,right},{1,same},{1,left},{1,right},{6,same},{1,left},{2,right},{8,same},{1,left},{1,right},{27,same},{1,right},{14,same},{1,left},{7,right},{9,same},{1,left},{1,same},{1,left},{1,right},{14,same},{2,right},{1,same},{1,left},{7,same},{2,left},{1,right},{6,same},{1,left},{1,right},{1,same},{1,left},{2,right},{1,same},{2,right},{1,same},{2,left},{1,same},{12,left},{1,right},{1,same},{4,left},{8,right},{1,same},{4,right},{3,same},{12,left},{1,right},{1,same},{4,left},{1,right},{1,same},{9,left},{7,right},{23,same},{1,right},{75,same},{1,left},{2,same},{1,left},{1,right},{3,same},{1,right},{1,same},{1,right},{4,same},{1,left},{2,same},{1,left},{1,right},{3,same},{1,left},{3,same},{1,left},{1,right},{14,same},{1,right},{24,same},{1,left},{1,right},{2,same},{2,right},{2,same},{1,left},{10,same},{6,right},{26,same},{1,left},{1,right},{17,same},{2,left},{2,right},{2,same},{1,left},{1,right},{7,same},{4,left},{4,right},{22,same},{1,right},{19,same},{1,left},{1,right},{5,same},{1,right},{5,same},{1,left},{1,right},{11,same},{1,left},{2,right},{56,same},{15,right},{2,same},{1,left},{2,right},{3,same},{1,left},{24,same},{1,left},{1,right},{20,same},{4,left},{9,right},{1,same},{1,left},{1,right},{1,same},{7,left},{3,right},{6,same},{8,left},{3,right},{5,same},{2,left},{3,right},{7,same},{1,left},{1,right},{3,same},{1,left},{1,right},{1,same},{3,left},{3,right},{5,same},{2,left},{2,right},{2,same},{3,left},{4,right},{1,same},{2,left},{1,right},{3,same},{2,left},{2,right},{9,same},{1,left},{1,right},{2,same},{3,left},{1,right},{1,same},{2,left},{1,same},{1,left},{1,right},{1,same},{3,left},{1,right},{6,same},{3,left},{4,right},{1,same},{4,left},{4,right},{2,same},{4,left},{1,right},{1,same},{1,left},{1,right},{1,same},{5,left},{1,right},{2,same},{1,left},{1,right},{1,same},{1,left},{2,right},{2,same},{13,left},{3,right},{1,same},{1,left},{1,right},{1,same},{8,left},{2,right},{2,same},{2,left},{3,right},{1,same},{8,left},{8,right},{1,same},{5,left},{5,right},{30,same},{19,right},{7,same},{1,left},{1,right},{8,same},{2,right},{2,same},{1,left},{2,same},{1,left},{6,same},{1,left},{1,right},{5,same},{9,left},{4,right},{3,same},{1,left},{9,right},{2,same},{1,left},{3,right},{1,same},{4,left},{4,right},{2,same},{1,left},{4,same},{1,left},{1,right},{9,same},{2,left},{3,right},{4,same},{2,left},{2,same},{1,left},{1,right},
#endif
	{  0 }
};

// affine transformation -- draw at (0,0), then translate to (0.1, 0.1)
// so as to avoid scaling the border
// gtk_layout_freeze / thaw
// group all the items
// hopefully we can scale the whole group in one go
// draw each grouped overview at (0,0) and scale then translate to final positions

void
draw_rect (double window_height, GnomeCanvasGroup *root_group)
{
	const gulong same_colour	= 0xffffffff;
	const gulong left_colour	= 0xff0000ff;
	const gulong right_colour	= 0xdfdf00ff;

	double ly1 = 0.0;
	double ly2 = 0.0;
	double ry1 = 0.0;
	double ry2 = 0.0;

	double lsize = 0.0;
	double rsize = 0.0;

	double translate[6];
	//double scale[6];

	GtkLayout *layout = NULL;
	gulong colour;
	double unit;
	double size;
	int type;
	int i;
	GnomeCanvasItem *item = NULL;
	GnomeCanvasGroup *lgroup = NULL;
	GnomeCanvasGroup *rgroup = NULL;

	lgroup = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root_group, gnome_canvas_group_get_type(), NULL));
	rgroup = GNOME_CANVAS_GROUP (gnome_canvas_item_new (root_group, gnome_canvas_group_get_type(), NULL));

	layout = GTK_LAYOUT (GNOME_CANVAS_ITEM (root_group)->canvas);
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

	unit = (window_height - (y_indent * 2)) / (MAX (lsize, rsize));

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
			item = gnome_canvas_item_new (lgroup, gnome_canvas_rect_get_type(),
				"x1", 0.0, "y1", ly1, "x2", bar_width, "y2", ly2, "fill_color_rgba", colour, NULL);
		}
		if ((type == same) || (type == right))
		{
			ry2 += size;
			item = gnome_canvas_item_new (rgroup, gnome_canvas_rect_get_type(),
				"x1", 0.0, "y1", ry1, "x2", bar_width, "y2", ry2, "fill_color_rgba", colour, NULL);
		}

		ly1 = ly2;
		ry1 = ry2;
	}

	//art_affine_scale     (scale, 2.0, 0.5);
	//art_affine_identity  (scale);
	//art_affine_translate (translate, 10.0, 10.0);
	//art_affine_multiply  (translate, scale, translate);

	art_affine_translate (translate, x_indent, y_indent);
	gnome_canvas_item_affine_absolute (GNOME_CANVAS_ITEM (lgroup), translate);

	art_affine_translate (translate, window_width - x_indent - bar_width, y_indent);
	gnome_canvas_item_affine_absolute (GNOME_CANVAS_ITEM (rgroup), translate);

	gtk_layout_thaw (layout);
}

