#ifndef _BRANCH_H_
#define _BRANCH_H_

#include <gtk/gtk.h>

class CBranch
{
public:
	CBranch (GtkWidget *tree);
	virtual ~CBranch();

	void SetName (char *name);
	int  AddLeaf (char *dir, char *file);
	gboolean Initialise (GtkCTreeNode *parent, char *name);

protected:
	GtkWidget *pTree;
	GtkCTreeNode *pParent;
	GPtrArray *m_pArray;
	GString   *m_pName;

	GdkPixmap *pixmap_open;
	GdkPixmap *pixmap_closed;

	GdkBitmap *mask_open;
	GdkBitmap *mask_closed;
private:

};

#endif // _BRANCH_H_

