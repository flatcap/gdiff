/* $Revision: 1.8 $ */

#ifndef _DERIVED_H_
#define _DERIVED_H_

#include <gnome.h>
#include "diff.h"

#define GTK_TYPE_DIFF_TREE		(gtk_diff_tree_get_type ())
#define GTK_DIFF_TREE(obj)		GTK_CHECK_CAST (obj, gtk_diff_tree_get_type (), GtkDiffTree)
#define GTK_DIFF_TREE_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, gtk_diff_tree_get_type (), GtkDiffTreeClass)
#define GTK_IS_DIFF_TREE(obj)		GTK_CHECK_TYPE (obj, gtk_diff_tree_get_type ())

typedef enum
{
	eFileSame  = 1 << 0,		// Obviously these are mutually exclusive for files,
	eFileLeft  = 1 << 1,		// but directories can accumulate them.
	eFileRight = 1 << 2,
	eFileDiff  = 1 << 3,
	eFileType  = 1 << 4,
	eFileError = 1 << 5,

	eFileAll   = (1 << 6) - 1
} Status;

typedef struct	_GtkDiffTree		GtkDiffTree;
typedef struct	_GtkDiffTreeClass	GtkDiffTreeClass;

guint		gtk_diff_tree_get_type		(void);
//GtkWidget *	gtk_diff_tree_new_with_titles	(gint columns, gint tree_column, gchar *titles[], DiffOptions *diff);
GtkWidget *	gtk_diff_tree_new		(gint columns, gint tree_column, DiffOptions *diff);

//void		gtk_diff_tree_set_view		(GtkDiffTree *tree, Status status);
//void		gtk_diff_tree_compare		(GtkDiffTree *tree, char *left, char *right);

// Signal handlers

struct _GtkDiffTree
{
	GtkCTree ctree;

	DiffOptions *diff;
	GnomeMDIChild *mdi_child;

	char	*left;
	char	*right;
	Status	 view;

	GNode	*root;

	gint flag1;
};

struct _GtkDiffTreeClass
{
	GtkCTreeClass parent_class;

	// Signal handlers
};

#endif // _DERIVED_H_

