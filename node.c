#include <stddef.h>
#include <string.h>
#include <gtk/gtk.h>
#include "node.h"

TreeNode * tree_node_new (char *name, Status status)
{
	TreeNode *node = (TreeNode*) g_malloc (sizeof (TreeNode));

	if (node)
	{
		node->name   = g_strdup (name);
		node->status = status;
	}
	
	return node;
}

void tree_node_free (TreeNode *node)
{
	if (node)
	{
		g_free (node->name);
		g_free (node);
	}
}

void tree_print (GNode *node, int depth)
{
	static char *space = "                                                ";
	TreeNode    *data  = NULL;
	GNode       *child = NULL;

	g_return_if_fail (node);
	//g_return_if_fail (node->data);

	child = node->children;
	while (child)
	{
		data = (TreeNode*) child->data;
		if (data)
		{
			g_print ("%.*s%s (%d)\n", depth, space, data->name, data->status);
			tree_print (child, depth + 2);
		}
		child = child->next;
	}
}

GNode * tree_node_find (GNode *node, char *name)
{
	TreeNode *tree  = NULL;
	GNode    *match = NULL;
	GNode    *child = NULL;

	g_return_val_if_fail (node, NULL);
	g_return_val_if_fail (name, NULL);

	child = node->children;
	while (child)
	{
		tree = (TreeNode*) child->data;
		if (tree)
		{
			if (strcmp (name, tree->name) == 0)
			{
				match = child;
				break;
			}
		}

		child = child->next;
	}

	return match;
}

void tree_node_add (GNode *parent, char *path, Status status)
{
	GNode    *node  = NULL;
	GString  *root  = NULL;
	GString  *rest  = NULL;
	char     *slash = NULL;
	TreeNode *data  = NULL;

	g_return_if_fail (parent);
	g_return_if_fail (path);

	root   = g_string_new (NULL);
	rest   = g_string_new (NULL);
	slash  = strchr (path, G_DIR_SEPARATOR);

	//g_print ("path: %s\n", path);
	g_string_assign (root, path);
	if (slash)
	{
		g_string_truncate (root, (slash - path));
		g_string_assign   (rest, (slash + 1));

		node = tree_node_find (parent, root->str);
		if (node)
		{
			//g_print ("recurse: %s (%d)\n", rest->str, status);
			tree_node_add (node, rest->str, status);
			data = (TreeNode*) node->data;
			data->status |= status;
		}
		else
		{
			//g_print ("new node: %s (%d)\n", root->str, status);
			data = tree_node_new (root->str, status);
			node = g_node_append_data (parent, data);
			tree_node_add (node, rest->str, status);
		}
	}
	else
	{
		//g_print ("new leaf: %s (%d)\n", root->str, status);
		data = tree_node_new (root->str, status);
		node = g_node_append_data (parent, data);
	}

	g_string_free (root, TRUE);
	g_string_free (rest, TRUE);

	//tree_print (g_node_get_root (parent), 0);
}
