#ifndef _TREE_H_
#define _TREE_H_

#include <gtk/gtk.h>

class CTree
{
public:
	CTree (GtkWidget *parent);
	virtual ~CTree();

	gboolean AddBranch (char *branch);
	gboolean AddLeaf   (char *leaf);

protected:

private:
	GtkWidget *parent;
	GtkWidget *tree;

	GdkPixmap *pixmap_open;
	GdkPixmap *pixmap_closed;
	GdkPixmap *pixmap_leaf;

	GdkBitmap *mask_open;
	GdkBitmap *mask_closed;
	GdkBitmap *mask_leaf;

	GPtrArray *m_pBranches;
	GPtrArray *m_pNames;
};

#endif // _TREE_H_

