/*
    global.c -- description
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

#include <stdlib.h>
#include "global.h"
#include "icons/open.xpm"
#include "icons/closed.xpm"
#include "icons/leaf.xpm"

/*----------------------------------------------------------------------------*/
gboolean global_init (GnomeMDI *mdi);
void global_close (void);
/*----------------------------------------------------------------------------*/

//______________________________________________________________________________
//
// Public data

GdkPixmap  *pixmap_open   = NULL;
GdkPixmap  *pixmap_closed = NULL;
GdkPixmap  *pixmap_leaf   = NULL;

GdkBitmap  *mask_open     = NULL;
GdkBitmap  *mask_closed   = NULL;
GdkBitmap  *mask_leaf     = NULL;

regex_t     reg_same;
regex_t     reg_diff;
regex_t     reg_only;
regex_t     reg_type;

//______________________________________________________________________________
//
// Private data

const char *re_diff = "Files \\(.*\\)/\\(.*\\) and \\(.*\\)/\\2 differ";
const char *re_same = "Files \\(.*\\)/\\(.*\\) and \\(.*\\)/\\2 are identical";
const char *re_only = "Only in \\(.*\\): \\(.*\\)\n";
const char *re_type = "File \\(.*\\)/\\(.*\\) is a \\(.*\\) while file \\(.*\\)/\\2 is a \\(.*\\)";

gboolean    initialised = FALSE;
Options    *options     = NULL;

//______________________________________________________________________________
//
// Public methods

gboolean
global_init (GnomeMDI *mdi)
{
	gboolean   regex_ok = FALSE;
	GtkWidget *widget   = NULL;

	if (!initialised)
	{
		g_return_val_if_fail (mdi != NULL, FALSE);

		regex_ok = ((regcomp (&reg_same, re_same, 0) == 0)	&&
			    (regcomp (&reg_diff, re_diff, 0) == 0)	&&
			    (regcomp (&reg_only, re_only, 0) == 0)	&&
			    (regcomp (&reg_type, re_type, 0) == 0));

		widget = GTK_WIDGET (gnome_mdi_get_active_window (mdi));
		if (widget)
		{
			pixmap_open	= gdk_pixmap_create_from_xpm_d (widget->window, &mask_open,   NULL, open_xpm);
			pixmap_closed	= gdk_pixmap_create_from_xpm_d (widget->window, &mask_closed, NULL, closed_xpm);
			pixmap_leaf	= gdk_pixmap_create_from_xpm_d (widget->window, &mask_leaf,   NULL, leaf_xpm);
		}

		initialised = pixmap_open && pixmap_closed && pixmap_leaf && regex_ok;
	}

	return initialised;
}

void
global_close (void)
{
	g_return_if_fail (initialised);

	regfree (&reg_same);
	regfree (&reg_diff);
	regfree (&reg_only);
	regfree (&reg_type);

	gdk_pixmap_unref (pixmap_open);
	gdk_pixmap_unref (pixmap_closed);
	gdk_pixmap_unref (pixmap_leaf);

	gdk_bitmap_unref (mask_open);
	gdk_bitmap_unref (mask_closed);
	gdk_bitmap_unref (mask_leaf);

	g_free (options);
}

Options *
global_get_options (PrefOption *list)
{
	Options *opt = NULL;

	opt = g_malloc0 (sizeof (Options));

	read_config_file (opt, list);
	//save_config_file (opt, list);

	return opt;
}

