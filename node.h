#ifndef _NODE_H_
#define _NODE_H_

#include <gtk/gtk.h>

enum Status
{
	eFileSame,
	eFileLeft,
	eFileRight,
	eFileDiff,
	eFileType,
	eFileError
};

class CNode
{
public:
	CNode (GtkWidget *pTree);
	virtual ~CNode();

	GtkCTreeNode * AddNode (GtkCTreeNode *pParent, GtkCTreeNode *pSibling, char *pName, Status eStatus);

protected:
	GtkWidget	*m_pTree;
	GtkCTreeNode	*m_pParent;
	GtkCTreeNode	*m_pSibling;
	GtkWidget	*m_pNode;

	GPtrArray	*m_pBranches;
	GPtrArray	*m_pNames;

	CNode * FindNode (char *pName);

private:

};

#endif // _NODE_H_

