/*
    node.c -- description
    Copyright (C) 1999  Richard Russon <richard.russon@ait.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Revision: 1.12 $ */

#include <stddef.h>
#include <string.h>
#include <gtk/gtk.h>
#include <regex.h>
#include "node.h"
#include "allocator.h"
#include "progress.h"
#include "global.h"

/*----------------------------------------------------------------------------*/
static TreeNode *	tree_node_find	(TreeNode *node, char *name);
//static TreeNode *	tree_node_new	(char *name, char *path, Status status);
//static void		tree_node_free	(TreeNode *node);

void			tree_node_add	(TreeNode *parent, char *orig_path, char **parts, Status status);
void			tree_print	(TreeNode *node, int depth);
TreeNode *		tree_parse_diff	(FILE *file, gchar *left, gchar *right, GnomeApp *app);
/*----------------------------------------------------------------------------*/

/*
static TreeNode *
tree_node_new (char *name, char *path, Status status)
{
	TreeNode *node = (TreeNode*) g_malloc (sizeof (TreeNode));

	if (node)
	{
		node->name   = g_strdup (name);
		node->path   = g_strdup (path);
		node->status = status;
	}
	
	return node;
}
*/

static GAllocator *
get_new_node_allocator (void)
{
	GAllocator *alloc = NULL;

	alloc = g_allocator_new ("TreeNode allocator", 1024);
	if (alloc)
	{
		alloc->type      = G_ALLOCATOR_NODE;
		alloc->mem_chunk = g_mem_chunk_create (TreeNode, 1024, G_ALLOC_AND_FREE);
	}

	return alloc;
}

static char **
tree_parse_line (char *buffer, gchar *left, gchar *right, GString *path, Status *status)
{
	regmatch_t matches [MATCHES];
	Status     result = eFileError;
	char     **parts  = NULL;

	g_return_val_if_fail (buffer, NULL);
	g_return_val_if_fail (left,   NULL);
	g_return_val_if_fail (right,  NULL);
	g_return_val_if_fail (status, NULL);

	if (regexec (&reg_same, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileSame;
	}
	else if (regexec (&reg_diff, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileDiff;
	}
	else if (regexec (&reg_only, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileLeft;				// This may be overridden
	}
	else if (regexec (&reg_type, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileType;
	}
	else
	{
		result = eFileError;
	}

	if (result == eFileError)
	{
		g_string_truncate (path, 0);
	}
	else
	{
		int l = strlen (left);
		int r = strlen (right);

		GString *file = g_string_new (buffer + matches[2].rm_so);

		g_string_assign (path, buffer + matches[1].rm_so);
		g_string_truncate (path, matches[1].rm_eo - matches[1].rm_so);
		g_string_append (path, G_DIR_SEPARATOR_S);

		g_string_truncate (file, matches[2].rm_eo - matches[2].rm_so);
		g_string_append (path, file->str);
		g_string_free (file, TRUE);

		if (strncmp (left, path->str, l) == 0)
		{
			g_string_erase (path, 0, l + 1);			// Same, diff, type, and left only
		}								// include slash /
		else if (strncmp (right, path->str, r) == 0)
		{
			g_string_erase (path, 0, r + 1);			// include slash /
			result = eFileRight;					// Right-only
		}
		else
		{
			result = eFileError;
		}
	}

	parts     = g_strsplit (path->str, G_DIR_SEPARATOR_S, -1);
	(*status) = result;

	return parts;
}

TreeNode *
tree_parse_diff (FILE *file, gchar *left, gchar *right, GnomeApp *app)
{
	char        buffer [_POSIX_PATH_MAX * 2 + 50];
	char       **parts = NULL;
	TreeNode    *node  = NULL;
	GAllocator *alloc  = get_new_node_allocator();
	Status      status = 0;
	GString    *path   = g_string_new (NULL);
	char **temp;

	g_node_push_allocator (alloc);

	node = (TreeNode *) g_node_new (NULL);

	while (fgets (buffer, sizeof (buffer), file))
	{
		parts = tree_parse_line (buffer, left, right, path, &status);

		for (temp = parts; *temp; temp++)
			g_print ("%s ", *temp);
		g_print ("\n");

		tree_node_add (node, path->str, parts, status);

		g_strfreev (parts);
	}

	g_string_free (path, TRUE);
	g_node_pop_allocator();
	g_allocator_free (alloc);

	tree_print (node, 0);

	exit (0);
	return node;
}

/*
static void
tree_node_free (TreeNode *node)
{
	if (node)
	{
		g_free (node->name);
		g_free (node->path);
		g_free (node);
	}
}
*/

static TreeNode *
tree_node_find (TreeNode *tree, char *name)
{
	g_return_val_if_fail (tree != NULL, NULL);
	g_return_val_if_fail (name != NULL, NULL);

	tree = (TreeNode*) tree->gnode.children;
	while (tree)
	{
		if (strcmp (name, tree->name) == 0)
		{
			break;
		}

		tree = (TreeNode*) tree->gnode.next;
	}

	return tree;
}

void
tree_node_add (TreeNode *parent, char *orig_path, char **parts, Status status)
{
	TreeNode *node  = NULL;

	g_return_if_fail (parent);
	g_return_if_fail (orig_path);
	g_return_if_fail (parts);

	if (*parts)
	{
		node = tree_node_find (parent, *parts);
		if (node)
		{
			tree_node_add (node, orig_path, ++parts, status);

			node->status |= status;
		}
		else
		{
			node = (TreeNode*) g_node_new (NULL);

			node->name   = g_strdup  (g_basename (orig_path));
			node->path   = g_dirname (orig_path);
			node->status = status;

			node = (TreeNode*) g_node_append ((GNode*) parent, (GNode*) node);

			tree_node_add (node, orig_path, ++parts, status);
		}
	}
}

void
tree_print (TreeNode *node, int depth)
{
	static char *space = "                                                ";

	g_return_if_fail (node);

	node = (TreeNode*) node->gnode.children;
	while (node)
	{
		g_print ("%.*s%s (%d)\n", depth, space, node->name, node->status);

		tree_print (node, depth + 2);

		node = (TreeNode*) node->gnode.next;
	}
}

