#ifndef _TREE_H_
#define _TREE_H_

#include <gtk/gtk.h>
#include "node.h"

typedef struct _TreeDialog TreeDialog;

TreeDialog *	tree_dialog_new   (GtkWidget *parent);
gboolean	tree_dialog_parse (TreeDialog *tree, char *left, char *right);
gboolean	tree_dialog_draw  (TreeDialog *tree, Status status);
void		tree_dialog_free  (TreeDialog *tree);

struct _TreeDialog
{
	GtkWidget	*parent;
	GtkWidget	*dialog;
	GtkWidget	*ctree;
	char		*left;
	char		*right;
	GNode		*root;
	Status		 view;
};

#if 0
	construct with base_left, base_right, return a widget

	set_view_status -- change visibility of nodes

	get color and font info from global -- callbacks into main? ;
	construct the tree with a ptr to a config engine?

	by deriving a new tree we can override all the allocation routines
	when finished unref or free? -- MUST be unref to make sure the base
	classes are freed cleanly
#endif

#endif // _TREE_H_

