/*
    compare.h -- description
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

#ifndef _COMPARE_H_
#define _COMPARE_H_

#include <gnome.h>
#include "diff.h"

#define GTK_TYPE_COMPARE		(gtk_compare_get_type ())
#define GTK_COMPARE(obj)		GTK_CHECK_CAST (obj, gtk_compare_get_type (), GtkCompare)
#define GTK_COMPARE_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, gtk_compare_get_type (), GtkCompareClass)
#define GTK_IS_COMPARE(obj)		GTK_CHECK_TYPE (obj, gtk_compare_get_type ())

typedef struct	_GtkCompare		GtkCompare;
typedef struct	_GtkCompareClass	GtkCompareClass;
typedef struct	_CompareRow		CompareRow;
typedef struct	_CompareLine		CompareLine;

guint		gtk_compare_get_type	(void);
GtkWidget *	gtk_compare_new		(DiffOptions *diff);

//void		gtk_compare (GtkCompare *compare);

// Signal handlers

struct _GtkCompare
{
	GtkCList clist;

	DiffOptions *diff;
	GnomeMDIChild *mdi_child;

	gint flag1;
	guint drawn;
	//XXX pointer to parent tree
};

struct _GtkCompareClass
{
	GtkCListClass parent_class;

	// Signal handlers
};

struct _CompareLine
{
	GList     glist;

	gchar    *raw_text;
	DiffType  type;
};

struct _CompareRow
{
	GtkCListRow  clist_row;

	CompareLine *line;
};

#endif // _COMPARE_H_

