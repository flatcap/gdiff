#ifndef _LEAF_H_
#define _LEAF_H_

#include <gtk/gtk.h>

class CLeaf
{
public:
	CLeaf (GtkWidget *pTree);
	virtual ~CLeaf();

	gboolean Initialise (GtkCTreeNode *pParent, char *name);

protected:
	GtkWidget *tree;
	GtkWidget *parent;

private:
};

#endif // _LEAF_H_

