void
size_request (GtkWidget *widget, GtkRequisition *requisition)
{
	//g_print ("request %dx%d\n", requisition->width, requisition->height);
}

void
size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
	//int    i = MIN (allocation->width, allocation->height);
	//double d = i;
	//int width  = allocation->width;
	//int height = allocation->height;
	//int x      = allocation->x;
	//int y      = allocation->y;
	//double d;

	//g_print ("pixels / unit %d\n", i);

	//width = MAX (width, 200);

	//XXX g_print ("alloc   %dx%d at (%d, %d)\n", width, height, x, y);

	//d = height;
	//d /= MIN_SIZE;
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

	gtk_signal_connect (GTK_OBJECT (canvas), "size_request",  size_request,  NULL);
	gtk_signal_connect (GTK_OBJECT (canvas), "size_allocate", size_allocate, NULL);


