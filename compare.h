/* $Id$ */

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

typedef struct _Compare Compare;

guint		gtk_compare_get_type	(void);
GtkWidget *	gtk_compare_new		(DiffOptions *diff);

void		gtk_compare (GtkCompare *compare);

// Signal handlers

struct _GtkCompare
{
	GtkCList clist;

	DiffOptions *diff;
	GnomeMDIChild *mdi_child;

	gint flag1;
};

struct _GtkCompareClass
{
	GtkCListClass parent_class;

	// Signal handlers
};

struct _Compare
{
	char *left;
	char *right;

	gpointer data;	// don't know what format, yet...
};


#endif // _COMPARE_H_

