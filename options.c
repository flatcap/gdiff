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

#include "config.h"
#include "options.h"
#include "style.h"
#include "global.h"

//XXX still missing tab size, exclusions list and extra options for diff
//XXX will need to be done manually

#define PIXEL_DEFAULT	(-1)
#define PIXEL_COLOUR	(0)

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
void save_config_file (Options *options, PrefOption *list);

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
check_toggled (GtkToggleButton *button, guint *number)
{
	GtkWidget *props = NULL;

	g_return_if_fail (button != NULL);
	g_return_if_fail (number != NULL);

	(*number) = !(*number);

	props = gtk_widget_get_ancestor (GTK_WIDGET (button), gnome_property_box_get_type());
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
	//XXX gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), (*option == 0));

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
	GtkWidget *props  = NULL;
	int       *option = NULL;

	g_return_if_fail (radio != NULL);

	option = gtk_object_get_user_data (GTK_OBJECT (radio));

	g_return_if_fail (option != NULL);

	(*option) = GPOINTER_TO_UINT (data);

	props = gtk_widget_get_ancestor (GTK_WIDGET (radio), gnome_property_box_get_type());
	if (props)
	{
		gnome_property_box_changed (GNOME_PROPERTY_BOX (props));
	}
}

static int
add_radio (GtkContainer *cont, PrefOption *list, Options *options)
{
	GtkToggleButton *toggle = NULL;
	GtkWidget       *hbox   = NULL;
	GtkRadioButton  *radio  = NULL;
	gchar           *ptr    = (gchar*) options;
	guint           *option = NULL;
	int	         count  = 0;

	ptr += list->offset;
	option = (guint*)ptr;

	for (; list->type == PrefRadio; list++, count++)
	{
		hbox  = gtk_hbox_new (FALSE, GNOME_PAD_SMALL);
		radio = GTK_RADIO_BUTTON (gtk_radio_button_new_with_label_from_widget (radio, list->label));

		gtk_object_set_user_data (GTK_OBJECT (radio), option);
		gtk_signal_connect (GTK_OBJECT (radio), "pressed", radio_pressed, GUINT_TO_POINTER (count));

		gtk_box_pack_start (GTK_BOX (hbox), GTK_WIDGET (radio), FALSE, FALSE, GNOME_PAD_SMALL);
		gtk_container_add (cont, hbox);
	}

	if (((*option) >= 0)   &&
	     (*option < count) &&
	     (radio != NULL))
	{
		// The last button is first in the group!
		toggle = GTK_TOGGLE_BUTTON (g_slist_nth_data (radio->group, count - 1 - (*option)));

		gtk_toggle_button_set_active (toggle, TRUE);
	}

	return count;
}

static void
colour_changed (GnomeColorPicker *picker, GtkWidget *entry, guint r, guint g, guint b, gboolean fg)
{
	GdkColor     colour = { PIXEL_COLOUR, r, g, b };
	PrefColours *option = NULL;
	GtkWidget   *props  = NULL;
	GtkStyle    *style  = NULL;

	g_return_if_fail (picker != NULL);
	g_return_if_fail (entry  != NULL);

	option = gtk_object_get_user_data (GTK_OBJECT (picker));
	style  = gtk_style_copy (gtk_widget_get_style (entry));	// no ref-counting on get

	if (fg)
	{
		option->fg = colour;
		style->fg[GTK_STATE_INSENSITIVE] = colour;
	}
	else
	{
		option->base = colour;
		style->base[GTK_STATE_INSENSITIVE] = colour;
	}

	gtk_widget_set_style (entry, style);			// ref-count +1
	gtk_style_unref (style);
	style = NULL;

	props = gtk_widget_get_ancestor (GTK_WIDGET (picker), gnome_property_box_get_type());
	if (props)
	{
		gnome_property_box_changed (GNOME_PROPERTY_BOX (props));
	}
}

static void
fg_changed (GnomeColorPicker *picker, guint r, guint g, guint b, guint a, GtkWidget *entry)
{
	colour_changed (picker, entry, r, g, b, TRUE);
}

static void
bg_changed (GnomeColorPicker *picker, guint r, guint g, guint b, guint a, GtkWidget *entry)
{
	colour_changed (picker, entry, r, g, b, FALSE);
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
	GtkStyle    *style  = NULL;
	PrefColours *colour = NULL;
	char        *ptr    = NULL;
	char        *temp   = NULL;

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

	temp = g_strdup_printf ("Pick a foreground colour for '%s'", list->label);
	gnome_color_picker_set_title (GNOME_COLOR_PICKER (fg), temp);
	g_free (temp);

	temp = g_strdup_printf ("Pick a background colour for '%s'", list->label);
	gnome_color_picker_set_title (GNOME_COLOR_PICKER (base), temp);
	g_free (temp);

	gtk_object_set_user_data (GTK_OBJECT (fg),   colour);
	gtk_object_set_user_data (GTK_OBJECT (base), colour);

	gtk_signal_connect (GTK_OBJECT (fg),   "color_set", fg_changed, entry);
	gtk_signal_connect (GTK_OBJECT (base), "color_set", bg_changed, entry);

	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_start (GTK_BOX (hbox), dummy, TRUE,  TRUE,  GNOME_PAD_SMALL);
	gtk_box_pack_end   (GTK_BOX (hbox), entry, FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_end   (GTK_BOX (hbox), base,  FALSE, FALSE, GNOME_PAD_SMALL);
	gtk_box_pack_end   (GTK_BOX (hbox), fg,    FALSE, FALSE, GNOME_PAD_SMALL);

	gtk_container_add (cont, hbox);
	gtk_widget_show_all (GTK_WIDGET (cont));

	gtk_widget_set_sensitive (entry, FALSE);
	gtk_entry_set_text (GTK_ENTRY (entry), _("Sample text"));

	style = gtk_style_copy (gtk_widget_get_style (entry));	// no ref-counting on get

	style->fg  [GTK_STATE_INSENSITIVE] = style->fg  [GTK_STATE_NORMAL];//these THREE are nec
	style->bg  [GTK_STATE_INSENSITIVE] = style->bg  [GTK_STATE_NORMAL];
	style->base[GTK_STATE_INSENSITIVE] = style->base[GTK_STATE_NORMAL];

	if (colour->fg.pixel == PIXEL_DEFAULT)
	{
		colour->fg				= style->fg[GTK_STATE_NORMAL];
		colour->fg.pixel			= PIXEL_DEFAULT;
	}
	else
	{
		style->fg[GTK_STATE_INSENSITIVE]	= colour->fg;
		colour->fg.pixel			= PIXEL_COLOUR;
	}

	if (colour->base.pixel == PIXEL_DEFAULT)
	{
		colour->base				 = style->base[GTK_STATE_NORMAL];
		colour->base.pixel			 = PIXEL_DEFAULT;
	}
	else
	{
		style->base[GTK_STATE_INSENSITIVE]	 = colour->base;
		colour->base.pixel			 = PIXEL_COLOUR;
	}

	gtk_widget_set_style (entry, style);

	gnome_color_picker_set_i16 (GNOME_COLOR_PICKER (fg),   colour->fg.red,   colour->fg.green,   colour->fg.blue,   0);
	gnome_color_picker_set_i16 (GNOME_COLOR_PICKER (base), colour->base.red, colour->base.green, colour->base.blue, 0);

	return 1;
}

static void
apply_signal (GnomePropertyBox *props, gint page_num, Options *options)
{
	g_print ("apply_signal (%d)\n", page_num);
	if (page_num != -1)
	{
		save_config_file (options, options_list);
	}
}

static void
help_signal (GnomePropertyBox *props, gint page_num)
{
	static GnomeHelpMenuEntry help = { PACKAGE, NULL };
	GtkWidget *notebook = NULL;
	GtkWidget *page     = NULL;
	GtkWidget *label    = NULL;

	g_print ("props_help_clicked\n");

	notebook = props->notebook;
	if (notebook)
	{
		page  = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), page_num);
		label = gtk_notebook_get_tab_label (GTK_NOTEBOOK (notebook), page);

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

static void
add_exlusions_page (GnomePropertyBox *props)
{
	GtkWidget *frame   = gtk_frame_new ("Exclusions");
	GtkWidget *padding = gtk_vbox_new  (FALSE, GNOME_PAD_SMALL);
	GtkWidget *label   = gtk_label_new ("Exclusions");
	GtkWidget *vbox    = gtk_vbox_new  (FALSE, GNOME_PAD_BIG);

	gtk_box_pack_start (GTK_BOX (padding), vbox, TRUE, TRUE, GNOME_PAD_SMALL);

	gtk_container_add  (GTK_CONTAINER (frame), padding);

	gnome_property_box_append_page (props, frame, label);
}

GtkWidget *
get_preferences (GtkWindow *parent, PrefsPage /*XXX unused*/page_sel)
{
	GnomePropertyBox *props = NULL;
	PrefOption       *list   = NULL;
	GtkContainer     *cont  = NULL;
	GtkContainer     *page  = NULL;
	Options		 *options = NULL;

	//XXX NEED TO COPY OPTIONS FIRST, then commit / emit on apply/OK

	props = GNOME_PROPERTY_BOX (gnome_property_box_new());

	options = global_get_options (options_list);

	gtk_object_set_user_data (GTK_OBJECT (props), options);

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

		default:	g_assert_not_reached();
				list++;
				break;
		}
	}

	add_style_page     (props);
	add_exlusions_page (props);

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
			gnome_config_set_int (key, *((guint*) (ptr)));
			break;

		case PrefStyle:
		{
			char *str = NULL;
			char *sfg = NULL;
			char *sbase = NULL;
			PrefColours *colour = (PrefColours*) ptr;

			if (colour->fg.pixel == PIXEL_DEFAULT)
			{
				sfg = strdup ("default");
			}
			else
			{
				sfg = g_strdup_printf ("#%02x%02x%02x",
					colour->fg.red	 >> 8,
					colour->fg.green >> 8,
					colour->fg.blue	 >> 8);
			}

			if (colour->base.pixel == PIXEL_DEFAULT)
			{
				sbase = strdup ("default");
			}
			else
			{
				sbase = g_strdup_printf ("#%02x%02x%02x",
					colour->base.red   >> 8,
					colour->base.green >> 8,
					colour->base.blue  >> 8);
			}

			str = g_strdup_printf ("%s,%s", sfg, sbase);

			gnome_config_set_string (key, str);
			g_free (sfg);
			g_free (sbase);
			g_free (str);
			break;
		}

		case PrefPage:
		case PrefFrame:
		default:
			g_assert_not_reached();
		}
	}

	g_free (key);
	gnome_config_sync();
}

void
read_config_file (Options *options, PrefOption *list)
{
	char		*section	= NULL;
	char		*key		= NULL;
	char		*ptr		= NULL;
	PrefColours	*colour		= NULL;
	char		*temp		= NULL;
	char		*delim		= NULL;
	GdkColor	 def_colour	= { PIXEL_DEFAULT, 0, 0, 0 }; // We chose a non-zero pixel value as meaning use default style

	for (; list->type; list++)
	{
		if (list->type == PrefPage)
		{
			section = list->rcfile;
			continue;
		}

		if ((list->type == PrefFrame) ||
		    (list->type == PrefLabel) ||
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
		case PrefRadio:
			*((guint*) (ptr)) = gnome_config_get_int (key);
			break;

		case PrefStyle:
			colour = (PrefColours*) ptr;
			colour->fg   = def_colour;
			colour->base = def_colour;

			temp = gnome_config_get_string (key);
			if (temp)
			{
				delim = strchr (temp, ',');
				*delim = 0;
				delim++;

				if (gdk_color_parse (temp,  &colour->fg))
				{
					colour->fg.pixel = PIXEL_COLOUR;
				}

				if (gdk_color_parse (delim, &colour->base))
				{
					colour->base.pixel = PIXEL_COLOUR;
				}

				g_free (temp);
			}
			break;

		case PrefPage:
		case PrefFrame:
		case PrefLabel:
		default:
			g_assert_not_reached();
		}
	}

	g_free (key);
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

