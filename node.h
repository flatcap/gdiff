/* $Id$ */

#ifndef _NODE_H_
#define _NODE_H_

#include <gtk/gtk.h>
#include "derived.h"

typedef struct _TreeNode TreeNode;

struct _TreeNode
{
	char   *name;
	char   *path;
	Status  status;
};

TreeNode * tree_node_new  (char *name, char *path, Status status);
void       tree_node_free (TreeNode *node);
GNode *    tree_node_find (GNode *node,   char *name);
void       tree_node_add  (GNode *parent, char *path, Status status, char *orig_path);
void       tree_print     (GNode *node, int depth);

#endif // _NODE_H_

