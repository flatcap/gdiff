/*
    derived.h -- description
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

#ifndef _DERIVED_H_
#define _DERIVED_H_

#include <gnome.h>
#include "diff.h"
#include "node.h"

#define GTK_TYPE_DIFF_TREE		(gtk_diff_tree_get_type ())
#define GTK_DIFF_TREE(obj)		GTK_CHECK_CAST (obj, gtk_diff_tree_get_type (), GtkDiffTree)
#define GTK_DIFF_TREE_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, gtk_diff_tree_get_type (), GtkDiffTreeClass)
#define GTK_IS_DIFF_TREE(obj)		GTK_CHECK_TYPE (obj, gtk_diff_tree_get_type ())

typedef struct	_GtkDiffTree		GtkDiffTree;
typedef struct	_GtkDiffTreeClass	GtkDiffTreeClass;

guint		gtk_diff_tree_get_type		(void);
//GtkWidget *	gtk_diff_tree_new_with_titles	(gint columns, gint tree_column, gchar *titles[], DiffOptions *diff);
GtkWidget *	gtk_diff_tree_new		(gint columns, gint tree_column, DiffOptions *diff);
DiffOptions *	get_current_selection		(GtkDiffTree *tree);
void		gtk_diff_tree_save_list		(GtkDiffTree *tree);

//void		gtk_diff_tree_set_view		(GtkDiffTree *tree, Status status);
//void		gtk_diff_tree_compare		(GtkDiffTree *tree, char *left, char *right);

struct _GtkDiffTree
{
	GtkCTree ctree;

	DiffOptions *diff;
	GnomeMDIChild *mdi_child;	// do we need this more/less than mdi?

	char     *left;
	char     *right;
	Status    view;

	TreeNode *root;

	gint flag1;
	gboolean drawn;
	//XXX collection of compares
	//XXX selection?
	//XXX GnomeMDI *mdi;
	//XXX GtkWidget *view;		shortcut in case we wrap up anything more
};

struct _GtkDiffTreeClass
{
	GtkCTreeClass parent_class;

	// Signal handlers
};

#endif // _DERIVED_H_

