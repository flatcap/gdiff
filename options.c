/*
    options.c -- description
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

/* $Revision: 1.19 $ */

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
	GtkWidget *vbox  = gtk_vbox_new  (FALSE, GNOME_PAD_BIG);

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
add_label (GtkContainer *cont, PrefOption *list, Options *options)
{
	GtkWidget *hbox  = gtk_hbox_new (FALSE, GNOME_PAD_SMALL);
	GtkWidget *label = gtk_label_new (list->label);

	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_container_add (cont, hbox);

	return 1;
}

static int
add_check (GtkContainer *cont, PrefOption *list, Options *options)
{
	GtkWidget *hbox   = gtk_hbox_new (FALSE, GNOME_PAD_SMALL);
	GtkWidget *toggle = gtk_check_button_new_with_label (_(list->label));
	gchar     *ptr    = (gchar*) options;
	guint     *pint   = NULL;

	ptr += list->offset;
	pint = (guint*)ptr;

	(GTK_TOGGLE_BUTTON (toggle))->active = (*pint != 0);

	gtk_signal_connect (GTK_OBJECT (toggle), "toggled", check_toggled, ptr);

	gtk_box_pack_start (GTK_BOX (hbox), toggle, FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_container_add (cont, hbox);


	return 1;
}

//typedef _OptionsCallback OptionsCallback;

/*
typedef struct
{
	Options *options;
	//PrefOption *list;
	int offset;
	//PrefType type;
	int value;
} OptionsCallback;
STUPID STUPID STUPID -- where am I going to store all these structs?
*/

/*
	Check	offset
	Radio	offset, enum
	Style	offset
*/

static void
radio_pressed (GtkWidget *radio, gpointer data)
{
	g_print ("radio pressed %p\n", data);
}

static int
add_radio (GtkContainer *cont, PrefOption *list, Options *options)
{
	GtkWidget      *hbox  = NULL;
	GtkRadioButton *radio = NULL;
	gchar          *ptr   = (gchar*) options;
	guint          *pint  = NULL;
	int	        count = 0;

	ptr += list->offset;
	pint = (guint*)ptr;

	//(GTK_TOGGLE_BUTTON (toggle))->active = (*pint != 0);

	for (; list->type == PrefRadio; list++, count++)
	{
		hbox  = gtk_hbox_new (FALSE, GNOME_PAD_SMALL);
		radio = GTK_RADIO_BUTTON (gtk_radio_button_new_with_label_from_widget (radio, list->label));

		gtk_signal_connect (GTK_OBJECT (radio), "pressed", radio_pressed, GUINT_TO_POINTER (count));

		gtk_box_pack_start (GTK_BOX (hbox), GTK_WIDGET (radio), FALSE, FALSE, GNOME_PAD_SMALL);
		gtk_container_add (cont, hbox);
	}
	
	return count;
}

typedef struct _StyleGroup StyleGroup;

struct _StyleGroup
{
	GtkLabel	 *label;
	GnomeColorPicker *fg;
	GnomeColorPicker *base;
	GtkEntry	 *entry;
	Options		 *options;
	guint		 offset;
};

static Options *global_options = NULL;

static void
colour_changed (GnomeColorPicker *picker, guint r, guint g, guint b, gboolean fg)
{
	StyleGroup  *group  = NULL;
	PrefColours *colour = NULL;
	gchar       *ptr    = NULL;
	GtkWidget   *props  = NULL;
	GtkStyle    *style  = NULL;

	g_print ("colour_changed %d\n", fg);

	group = gtk_object_get_user_data (GTK_OBJECT (picker));

	ptr = (char*) group->options;
	ptr += group->offset;
	colour = (PrefColours*) ptr;

	//XXX unref the old style
	style = gtk_style_copy (gtk_widget_get_style (GTK_WIDGET (group->entry))); //XXX what the ....
	if (fg)
	{
		gnome_color_picker_get_i16 (GNOME_COLOR_PICKER (picker), &colour->fg.red, &colour->fg.green, &colour->fg.blue, NULL);
		style->fg[GTK_STATE_INSENSITIVE] = colour->fg;
	}
	else
	{
		gnome_color_picker_get_i16 (GNOME_COLOR_PICKER (picker), &colour->base.red, &colour->base.green, &colour->base.blue, NULL);
		style->base[GTK_STATE_INSENSITIVE] = colour->base;
	}

	gtk_widget_set_style (GTK_WIDGET (group->entry), style);

	props = gtk_widget_get_ancestor (GTK_WIDGET (picker), gnome_property_box_get_type());
	if (props)
	{
		gnome_property_box_changed (GNOME_PROPERTY_BOX (props));
	}
}

static void
fg_changed (GnomeColorPicker *picker, guint r, guint g, guint b, guint a) // actually gushort
{
	colour_changed (picker, r, g, b, TRUE);
}

static void
bg_changed (GnomeColorPicker *picker, guint r, guint g, guint b, guint a) // actually gushort
{
	colour_changed (picker, r, g, b, FALSE);
}

static int
add_style (GtkContainer *cont, PrefOption *list, Options *options)
{
	GtkWidget   *hbox   = NULL;
	GtkWidget   *label  = NULL;
	GtkWidget   *dummy  = NULL;
	GtkWidget   *entry  = NULL;
	GtkWidget   *fg     = NULL;
	GtkWidget   *base   = NULL;
	//GtkWidget   *check  = NULL;
	GtkStyle    *style  = NULL;
	PrefColours *colour = NULL;
	char        *ptr    = NULL;
	StyleGroup  *group  = NULL;

	hbox = gtk_hbox_new (FALSE, GNOME_PAD_SMALL);

	ptr = (char *) options;
	ptr += list->offset;
	colour = (PrefColours*) ptr;

	// We use a dummy label "" for padding
	label = gtk_label_new (list->label);
	dummy = gtk_label_new ("");
	entry = gtk_entry_new();
	fg    = gnome_color_picker_new();
	base  = gnome_color_picker_new();
	//check = gtk_check_button_new();

	group = g_malloc (sizeof (StyleGroup));		//XXX who's going to free this?
	g_return_val_if_fail (group != NULL, 0);

	group->label	= GTK_LABEL (label);
	group->fg	= GNOME_COLOR_PICKER (fg);
	group->base	= GNOME_COLOR_PICKER (base);
	group->entry	= GTK_ENTRY (entry);
	group->options	= options;
	group->offset	= list->offset;

	gtk_object_set_user_data (GTK_OBJECT (label),  group);
	gtk_object_set_user_data (GTK_OBJECT (fg),     group);
	gtk_object_set_user_data (GTK_OBJECT (base),   group);
	gtk_object_set_user_data (GTK_OBJECT (entry),  group);

	gtk_signal_connect (GTK_OBJECT (fg), "color_set", fg_changed, NULL);
	gtk_signal_connect (GTK_OBJECT (base), "color_set", bg_changed, NULL);

	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (hbox), dummy, TRUE,  TRUE,  GNOME_PAD_SMALL);
	gtk_box_pack_end   (GTK_BOX (hbox), entry, FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_end   (GTK_BOX (hbox), base,  FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_end   (GTK_BOX (hbox), fg,    FALSE, FALSE, GNOME_PAD_SMALL);
	//gtk_box_pack_end   (GTK_BOX (hbox), check, FALSE, FALSE, GNOME_PAD_SMALL); //XXX enable colours

	gtk_entry_set_text (GTK_ENTRY (entry), _("Sample text"));
	gtk_widget_set_sensitive (entry, FALSE);

	style = gtk_style_new();
	style->fg  [GTK_STATE_INSENSITIVE] = colour->fg;
	style->base[GTK_STATE_INSENSITIVE] = colour->base;

	gtk_widget_set_style (entry, style);

	gnome_color_picker_set_i16 (GNOME_COLOR_PICKER (fg),   colour->fg.red, colour->fg.green, colour->fg.blue, 0);
	gnome_color_picker_set_i16 (GNOME_COLOR_PICKER (base), colour->base.red, colour->base.green, colour->base.blue, 0);

	//gtk_widget_set_sensitive (GTK_WIDGET (fg),   FALSE);
	//gtk_widget_set_sensitive (GTK_WIDGET (base), FALSE);

	gtk_container_add (cont, hbox);

	return 1;
}

static int
add_list (GtkContainer *cont, PrefOption *list, Options *options)
{
	gtk_container_add (cont, gtk_list_new());
	return 1;
}

void save_config_file (Options *options, PrefOption *list);

static void
apply_signal (GtkWidget *button, Options *options)
{
	g_print ("apply_signal (%p)\n", options);
	if (options != GUINT_TO_POINTER (-1))
	{
		save_config_file (global_options, options_list);
	}
}

static void
help_signal (GtkWidget *button, gpointer data)
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
get_preferences (GtkWindow *parent, PrefsPage /*XXX unused*/page_sel)
{
	GnomePropertyBox *props = NULL;
	PrefOption       *list   = NULL;
	GtkContainer     *cont  = NULL;
	GtkContainer     *page  = NULL;
	Options		 *options = NULL;

	options = options_get_default (options_list);
	global_options = options;

	props = GNOME_PROPERTY_BOX (gnome_property_box_new());

	gtk_signal_connect (GTK_OBJECT (props), "apply", apply_signal, options);
	gtk_signal_connect (GTK_OBJECT (props), "help",  help_signal,  NULL);

	for (list = options_list; list->type;)
	{
		switch (list->type)
		{
		case PrefPage:	page = add_page (props, list);
				cont = page;
				list++;
				break;

		case PrefFrame:	cont = add_frame (page, list);
				list++;
				break;

		case PrefLabel:	list += add_label (cont, list, options);
				break;

		case PrefCheck:	list += add_check (cont, list, options);
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
		{
			char *str = NULL;
			PrefColours *colour = (PrefColours*) ptr;
			//g_print ("[%s] %s = %p\n", section, key, (ptr));

			str = g_strdup_printf ("#%02x%02x%02x,#%02x%02x%02x",
				colour->fg.red	 >> 8,
				colour->fg.green >> 8,
				colour->fg.blue	 >> 8,
				colour->base.red	 >> 8,
				colour->base.green >> 8,
				colour->base.blue	 >> 8);
			//g_print ("Colour = %s\n", str);
			gnome_config_set_string (key, str);
			g_free (str);
			break;
		}

		case PrefList:
			//g_print ("[%s] %s = (null)\n", section, key);
			gnome_config_set_int (key, 0);
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
	PrefColours *colour = NULL;
	char *temp = NULL;
	char *delim = NULL;
	
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
			//g_print ("[%s] %s = %d\n", section, key, *((guint*) (ptr)));
			break;

		case PrefStyle:
		{
			colour = (PrefColours*) ptr;
			temp = gnome_config_get_string (key);
			delim = strchr (temp, ',');		// XXX if (temp) else gtk_style_new!
			*delim = 0;
			delim++;
			//g_print ("[%s] %s = %p\n", section, key, ptr);
			gdk_color_parse (temp,  &colour->fg);
			gdk_color_parse (delim, &colour->base);
			g_free (temp);
			break;
		}

		case PrefList:
			*((guint*) (ptr)) = gnome_config_get_int (key);
			//g_print ("[%s] %s = %d\n", section, key, *((guint*) (ptr)));
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

	if (0)
	{
		GdkColor red  = { 0, 65535, 0,     0 };
		GdkColor blue = { 0,     0, 0, 65535 };

		opt->DirStyleSame.fg = red;
		opt->DirStyleSame.base = blue;
		opt->DirStyleLeft.fg = red;
		opt->DirStyleLeft.base = blue;
		opt->DirStyleRight.fg = red;
		opt->DirStyleRight.base = blue;
		opt->DirStyleDiff.fg = red;
		opt->DirStyleDiff.base = blue;
		opt->DirStyleError.fg = red;
		opt->DirStyleError.base = blue;
		opt->FileStyleLeft.fg = red;
		opt->FileStyleLeft.base = blue;
		opt->FileStyleRight.fg = red;
		opt->FileStyleRight.base = blue;
	}

	read_config_file (opt, list);
	//save_config_file (opt, list);

	return opt;
}

static void
destroy_prefs (GtkWidget *parent, GtkWidget **prefs) //XXX misc.c?
{
	g_return_if_fail (prefs != NULL);

	if (*prefs)
	{
		gtk_widget_destroy (*prefs);
		*prefs = NULL;
	}
}

void
show_preferences (GtkWindow *parent, PrefsPage page)
{
	static GtkWidget *prefs  = NULL;

	g_return_if_fail (parent != NULL);

	if (prefs)
	{
		gdk_window_show  (prefs->window);
		gdk_window_raise (prefs->window);
	}
	else
	{
		prefs = get_preferences (parent, page);
		if (prefs)
		{
			gtk_signal_connect (GTK_OBJECT (prefs),  "destroy", GTK_SIGNAL_FUNC (gtk_widget_destroyed), &prefs);
			gtk_signal_connect (GTK_OBJECT (parent), "destroy", GTK_SIGNAL_FUNC (destroy_prefs),        &prefs);
			gtk_widget_show_all (prefs);
		}
	}

	if ((page >=0) && (page < PageMax))
	{
		gtk_notebook_set_page (GTK_NOTEBOOK (GNOME_PROPERTY_BOX (prefs)->notebook), page);
	}
}

