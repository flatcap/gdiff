/*
    mdi.h -- description
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

/* $Revision: 1.11 $ */

#ifndef _MDI_H_
#define _MDI_H_

#include "options.h"
#include "diff.h"

GnomeMDI *	mdi_new			(gchar *appname, gchar *title);
void		mdi_add_diff		(GnomeMDI *mdi, DiffOptions *diff);
GtkWidget *	mdi_get_current_view	(GnomeMDI *mdi);
void		mdi_close		(GnomeMDI *mdi);

#endif // _MDI_H_

