/* $Revision: 1.10 $ */

#ifndef _MDI_H_
#define _MDI_H_

#include "options.h"
#include "diff.h"

GnomeMDI *	mdi_new			(gchar *appname, gchar *title);
void		mdi_add_diff		(GnomeMDI *mdi, DiffOptions *diff);
GtkWidget *	mdi_get_current_view	(GnomeMDI *mdi);
void		mdi_close		(GnomeMDI *mdi);

#endif // _MDI_H_

