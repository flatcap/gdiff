#ifndef _NODE_H_
#define _NODE_H_

#include <gtk/gtk.h>

enum _Status
{
	eFileSame  = 1 << 0,		// Obviously these are mutually exclusive for files,
	eFileLeft  = 1 << 1,		// but directories can accumulate them.
	eFileRight = 1 << 2,
	eFileDiff  = 1 << 3,
	eFileType  = 1 << 4,
	eFileError = 1 << 5,

	eFileAll   = (1 << 6) - 1
};

typedef enum   _Status   Status;

struct _TreeNode
{
	char   *name;
	Status  status;
};

typedef struct _TreeNode TreeNode;

TreeNode * tree_node_new  (char *name, Status status);
void       tree_node_free (TreeNode *node);
GNode *    tree_node_find (GNode *node,   char *name);
void       tree_node_add  (GNode *parent, char *path, Status status);
void       tree_print     (GNode *node, int depth);

#if 0
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

#endif

#endif // _NODE_H_

