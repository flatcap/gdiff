#ifndef _NODE_H_
#define _NODE_H_

#include <gtk/gtk.h>
#include "derived.h"

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

#endif // _NODE_H_

