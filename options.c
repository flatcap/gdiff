/* $Revision: 1.13 $ */

#include "config.h"
#include "options.h"

//XXX still missing tab size, exclusions list and extra options for diff
//XXX will need to be done manually

/*
this will be an object representing a diff's options
there will also an 'option' help by the global object (the defaults)

options_new will read from the config file
commit will save to cfg file

who will own the global options? refcount?
*/

/*----------------------------------------------------------------------------*/
//static Options * options_new (void);
//static gboolean options_save_options (Options *options);
//static gboolean options_set_options  (Options *options);
//static void options_free (Options *options);

Options * options_get_default (PrefOption *list);
/*----------------------------------------------------------------------------*/

void
options_save (Options *options)
{
}

/*
static Options *
options_new (void)
{
	return NULL;
}
*/

/*
static void
options_free (Options *options)
{
	g_free (options);
}
*/

/*
static gboolean
options_set_options  (Options *options)
{
	return FALSE;
}
*/

/*
static gboolean
options_save_options (Options *options)
{
	gboolean result = FALSE;

	if (options_set_options (options))
	{
		//save to config file
	}

	return result;
}
*/

#include "preferences.c"

static GtkContainer *
add_page (GnomePropertyBox *props, PrefOption *opt)
{
	GtkWidget *label = gtk_label_new (_(opt->label));
	GtkWidget *vbox  = gtk_vbox_new  (FALSE, GNOME_PAD);

	gnome_property_box_append_page (props, vbox, label);

	return GTK_CONTAINER (vbox);
}

static GtkContainer *
add_frame (GtkContainer *box, PrefOption *opt)
{
	GtkWidget *frame = gtk_frame_new (_(opt->label));
	GtkWidget *vbox  = gtk_vbox_new  (FALSE, GNOME_PAD_SMALL);

	gtk_box_pack_start (GTK_BOX (box), frame, FALSE, FALSE, GNOME_PAD_SMALL);

	gtk_container_add (GTK_CONTAINER (frame), vbox);

	return GTK_CONTAINER (vbox);
}

static void
check_toggled (GtkToggleButton *toggle_button, guint *number)
{
	GtkWidget *props;

	g_print ("check toggled %d\n", *number);
	*number = !*number;

	props = gtk_widget_get_ancestor (GTK_WIDGET (toggle_button), gnome_property_box_get_type());
	if (props)
	{
		gnome_property_box_changed (GNOME_PROPERTY_BOX (props));
	}

}

static int
add_check (GtkContainer *cont, PrefOption *list, Options *options)
{
	GtkWidget *toggle = gtk_check_button_new_with_label (_(list->label));
	gchar     *ptr    = (gchar*) options;
	guint     *pint   = NULL;

	ptr += list->offset;
	pint = (guint*)ptr;

	*pint = 42;

	(GTK_TOGGLE_BUTTON (toggle))->active = TRUE;

	gtk_signal_connect (GTK_OBJECT (toggle), "toggled", check_toggled, ptr);

	gtk_container_add (cont, toggle);

	return 1;
}

static int
add_label (GtkContainer *cont, PrefOption *list, Options *options)
{
	gtk_container_add (cont, gtk_label_new (list->label));
	return 1;
}

static int
add_radio (GtkContainer *cont, PrefOption *list, Options *options)
{
	GtkRadioButton *radio = NULL;
	int		count = 0;

	for (; list->type == PrefRadio; list++, count++)
	{
		radio = GTK_RADIO_BUTTON (gtk_radio_button_new_with_label_from_widget (radio, list->label));

		gtk_container_add (cont, GTK_WIDGET (radio));
	}
	
	return count;
}

static int
add_style (GtkContainer *cont, PrefOption *list, Options *options)
{
	gtk_container_add (cont, gtk_check_button_new_with_label (_(list->label)));
	return 1;
}

static int
add_list (GtkContainer *cont, PrefOption *list, Options *options)
{
	return 1;
}

static void
props_ok_clicked (GtkWidget *button, gpointer data)
{
	g_print ("props_ok_clicked\n");
}

static void
props_apply_clicked (GtkWidget *button, gpointer data)
{
	g_print ("props_apply_clicked\n");
}

static void
props_cancel_clicked (GtkWidget *button, gpointer data)
{
	g_print ("props_cancel_clicked\n");
}

static void
props_help_clicked (GtkWidget *button, GnomeHelpMenuEntry *menu)
{
	static GnomeHelpMenuEntry help = { PACKAGE, NULL };
	GtkWidget *props    = NULL;
	GtkWidget *notebook = NULL;
	GtkWidget *page     = NULL;
	GtkWidget *label    = NULL;
	int        num;

	g_print ("props_help_clicked\n");

	props = gtk_widget_get_ancestor (GTK_WIDGET (button), gnome_property_box_get_type());
	if (props)
	{
		notebook = GNOME_PROPERTY_BOX (props)->notebook;
	}

	if (notebook)
	{
		num   = gtk_notebook_get_current_page (GTK_NOTEBOOK (notebook));
		page  = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), num);
		label = gtk_notebook_get_tab_label (GTK_NOTEBOOK (notebook), page);

		//g_print ("label = %s\n", GTK_LABEL (label)->label);

		help.path = g_strdup_printf ("options-%s.html", GTK_LABEL (label)->label);

	}
	else
	{
		help.path = g_strdup ("options.html");
	}

	g_print ("help path = %s\n", help.path);
	gnome_help_display (NULL, &help);

	g_free (help.path);
}

GtkWidget *
get_preferences (GtkWindow *parent, PrefsPage page)
{
	static GnomeHelpMenuEntry help = { PACKAGE, "options.html" };
	GnomePropertyBox *props = NULL;
	PrefOption       *list   = NULL;
	GtkContainer     *cont  = NULL;
	Options		 *options = NULL;

	props = GNOME_PROPERTY_BOX (gnome_property_box_new());

	gtk_signal_connect (GTK_OBJECT (props->ok_button),     "clicked", props_ok_clicked, NULL);
	gtk_signal_connect (GTK_OBJECT (props->apply_button),  "clicked", props_apply_clicked, NULL);
	gtk_signal_connect (GTK_OBJECT (props->cancel_button), "clicked", props_cancel_clicked, NULL);
	gtk_signal_connect (GTK_OBJECT (props->help_button),   "clicked", props_help_clicked, &help);

	options = options_get_default (options_list);

	for (list = options_list; list->type;)
	{
		switch (list->type)
		{
		case PrefPage:	cont = add_page  (props, list);
				list++;
				break;

		case PrefFrame:	cont = add_frame (cont, list);
				list++;
				break;

		case PrefCheck:	list += add_check (cont, list, options);
				break;

		case PrefLabel:	list += add_label (cont, list, options);
				break;

		case PrefRadio:	list += add_radio (cont, list, options);
				break;

		case PrefStyle:	list += add_style (cont, list, options);
				break;

		case PrefList:	list += add_list  (cont, list, options);
				break;

		default:	g_assert_not_reached();
				list++;
				break;
		}
	}

	//set_default_page (page);
	gtk_window_set_transient_for (GTK_WINDOW (props), parent);

	return GTK_WIDGET (props);
}

void 
save_config_file (Options *options, PrefOption *list)
{
	char *section = NULL;
	char *key     = NULL;
	char *ptr     = NULL;
	
	for (; list->type; list++)
	{
		if (list->type == PrefPage)
		{
			section = list->rcfile;
			continue;
		}

		if ((list->type == PrefFrame) ||
		   ((list->type == PrefRadio) && (list->rcfile == NULL)))
		{
			continue;
		}

		g_free (key);
		key = g_strconcat ("/", PACKAGE, "/", section, "/", list->rcfile, NULL);
		ptr = (char *) options;
		ptr += list->offset;

		switch (list->type)
		{
		case PrefCheck:
		case PrefLabel:
		case PrefRadio:
			//g_print ("[%s] %s = %d\n", section, key, *((guint*) (ptr)));
			gnome_config_set_int (key, *((guint*) (ptr)));
			break;

		case PrefStyle:
			//g_print ("[%s] %s = %p\n", section, key, (ptr));
			gnome_config_set_int (key, (guint) (ptr));
			break;
			break;

		case PrefList:
			//g_print ("[%s] %s = (null)\n", section, key);
			gnome_config_set_int (key, 0);
			break;
			break;

		case PrefPage:
		case PrefFrame:
		default:
			//g_print ("%d\n", list->type);
			g_assert_not_reached();
		}
	}

	g_free (key);
	gnome_config_sync();
}

void 
read_config_file (Options *options, PrefOption *list)
{
	char *section = NULL;
	char *key     = NULL;
	char *ptr     = NULL;
	
	for (; list->type; list++)
	{
		if (list->type == PrefPage)
		{
			section = list->rcfile;
			continue;
		}

		if ((list->type == PrefFrame) ||
		   ((list->type == PrefRadio) && (list->rcfile == NULL)))
		{
			continue;
		}

		g_free (key);
		key = g_strconcat ("/", PACKAGE, "/", section, "/", list->rcfile, NULL);
		ptr = (char *) options;
		ptr += list->offset;

		switch (list->type)
		{
		case PrefCheck:
		case PrefLabel:
		case PrefRadio:
			*((guint*) (ptr)) = gnome_config_get_int (key);
			g_print ("[%s] %s = %d\n", section, key, *((guint*) (ptr)));
			break;

		case PrefStyle:
			*((guint*) (ptr)) = gnome_config_get_int (key);
			g_print ("[%s] %s = %p\n", section, key, ptr);
			break;
			break;

		case PrefList:
			*((guint*) (ptr)) = gnome_config_get_int (key);
			g_print ("[%s] %s = %d\n", section, key, *((guint*) (ptr)));
			break;
			break;

		case PrefPage:
		case PrefFrame:
		default:
			//g_print ("%d\n", list->type);
			g_assert_not_reached();
		}
	}

	g_free (key);
}

Options *
options_get_default (PrefOption *list)
{
	//XXX ask global for this first
	Options *opt = NULL;

	opt = g_malloc0 (sizeof (Options));

	opt->DirStyleSame	= gtk_style_new();
	opt->DirStyleLeft	= gtk_style_new();
	opt->DirStyleRight	= gtk_style_new();
	opt->DirStyleDiff	= gtk_style_new();
	opt->DirStyleError	= gtk_style_new();
	opt->FileStyleLeft	= gtk_style_new();
	opt->FileStyleRight	= gtk_style_new();

	read_config_file (opt, list);
	//save_config_file (opt, list);

	return opt;
}

