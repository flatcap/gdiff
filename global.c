#include <stdlib.h>
#include "global.h"
#include "icons/open.xpm"
#include "icons/closed.xpm"
#include "icons/leaf.xpm"

//______________________________________________________________________________
//
// Public

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
// Private

const char *re_diff = "Files \\(.*\\)/\\(.*\\) and \\(.*\\)/\\2 differ";
const char *re_same = "Files \\(.*\\)/\\(.*\\) and \\(.*\\)/\\2 are identical";
const char *re_only = "Only in \\(.*\\): \\(.*\\)\n";
const char *re_type = "File \\(.*\\)/\\(.*\\) is a \\(.*\\) while file \\(.*\\)/\\2 is a \\(.*\\)";

gboolean    initialised = FALSE;

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
		g_return_val_if_fail (mdi                != NULL, FALSE);
		g_return_val_if_fail (mdi->windows       != NULL, FALSE);
		g_return_val_if_fail (mdi->windows->data != NULL, FALSE);	// mdi has no windows!

		widget = GTK_WIDGET (mdi->windows->data);

		regex_ok = ((regcomp (&reg_same, re_same, 0) == 0)	&&
			    (regcomp (&reg_diff, re_diff, 0) == 0)	&&
			    (regcomp (&reg_only, re_only, 0) == 0)	&&
			    (regcomp (&reg_type, re_type, 0) == 0));

		pixmap_open	= gdk_pixmap_create_from_xpm_d (widget->window, &mask_open,   NULL, open_xpm);
		pixmap_closed	= gdk_pixmap_create_from_xpm_d (widget->window, &mask_closed, NULL, closed_xpm);
		pixmap_leaf	= gdk_pixmap_create_from_xpm_d (widget->window, &mask_leaf,   NULL, leaf_xpm);

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
}

