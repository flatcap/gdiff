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

Options * options_get_default (void);

#endif // _OPTIONS_H_

