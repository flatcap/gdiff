#ifndef _MDI_H_
#define _MDI_H_

#include "options.h"
#include "diff.h"

GnomeMDI *	mdi_new		(gchar *appname, gchar *title);
void		mdi_add_diff	(GnomeMDI *mdi, Options *options, DiffOptions *diff);
void		mdi_show_all	(GnomeMDI *mdi);

#endif // _MDI_H_

