/* $Id$ */

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <gnome.h>

typedef struct _Options Options;

struct _Options
{
	GdkColor left;
	GdkColor right;
	guint	flag1 : 1;
};

Options *	options_new (void);
Options *	options_get_default (void);
void		options_free (Options *options);

gboolean	options_set_options  (Options *options);
gboolean	options_save_options (Options *options);

typedef enum
{
	// Fonts
	diff_font,
	file_font,

	// Styles
	diff_left,
	diff_right,
	diff_same,
	diff_diff,
	file_left,
	file_right,

	// Sizes
	diff_window,
	file_window,

	// Flags
	mdi_mode,

} ConfigOption;

#endif // _OPTIONS_H_

