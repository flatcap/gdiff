#ifndef _NODE_H_
#define _NODE_H_

#include <gtk/gtk.h>

enum Status
{
	eFileSame,
	eFileLeft,
	eFileRight,
	eFileDiff,
	eFileError
};

class CNode
{
public:
	CNode (GtkWidget *pTree);
	virtual ~CNode();

	gboolean AddNode (GtkCTreeNode *pParent, char *pName, Status eStatus);

protected:
	GtkWidget	*m_pTree;
	GtkCTreeNode	*m_pParent;
	GtkWidget	*m_pNode;
	GString		*m_pName;

	GPtrArray	*m_pBranches;
	GPtrArray	*m_pNames;

	CNode * FindNode (char *pName);

private:

};

#endif // _NODE_H_

