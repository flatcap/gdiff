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

#endif // _TREE_H_

