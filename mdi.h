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

/* $Revision: 1.12 $ */

#ifndef _MDI_H_
#define _MDI_H_

#include <gnome.h>
#include "options.h"
#include "diff.h"

#define GTK_TYPE_MDI_DIFF_CHILD		(gtk_mdi_diff_child_get_type())
#define GTK_MDI_DIFF_CHILD(obj)		GTK_CHECK_CAST (obj, gtk_mdi_diff_child_get_type(), MDIDiffChild)
#define GTK_MDI_DIFF_CHILD_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, gtk_mdi_diff_child_get_type(), MDIDiffChildClass)
#define GTK_IS_MDI_DIFF_CHILD(obj)	GTK_CHECK_TYPE (obj, gtk_mdi_diff_child_get_type())

typedef struct	_MDIDiffChild		MDIDiffChild;
typedef struct	_MDIDiffChildClass	MDIDiffChildClass;

guint		gtk_mdi_diff_child_get_type	(void);
MDIDiffChild *	gtk_mdi_diff_child_new		(char *name, DiffOptions *diff);

GnomeMDI *	mdi_new			(gchar *appname, gchar *title);
void		mdi_add_diff		(GnomeMDI *mdi, DiffOptions *diff);
GtkWidget *	mdi_get_current_view	(GnomeMDI *mdi);
void		mdi_close		(GnomeMDI *mdi);
void		mdi_close_view		(GnomeMDI *mdi, GnomeMDIChild *child);

struct _MDIDiffChild
{
	GnomeMDIChild mdi_child;

	DiffOptions *diff_options;

	/*
	DiffType type;

	gchar *left_root;
	gchar *right_root;

	gchar *relative;

	gchar *left;
	gchar *right;

	gchar *last_error;
	*/
};

struct _MDIDiffChildClass
{
	GnomeMDIChildClass parent_class;
};

#endif // _MDI_H_

