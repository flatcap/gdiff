/*
    node.h -- description
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

/* $Revision: 1.10 $ */

#ifndef _NODE_H_
#define _NODE_H_

#include <gnome.h>

typedef struct _TreeNode    TreeNode;
typedef struct _DiffTreeRow DiffTreeRow;

//#include "derived.h"

typedef enum
{
	eFileSame  = 1 << 0,		// Obviously these are mutually exclusive for files,
	eFileLeft  = 1 << 1,		// but directories can accumulate them.
	eFileRight = 1 << 2,
	eFileDiff  = 1 << 3,
	eFileType  = 1 << 4,
	eFileError = 1 << 5,

	eFileAll   = (1 << 6) - 1
} Status;

struct _DiffTreeRow
{
	GtkCTreeRow row;

	char   *name;
	char *path;
	Status  status;

	TreeNode *node;
};

struct _TreeNode
{
	GNode        gnode;

	gchar       *name;
	gchar       *path;
	Status       status;

	DiffTreeRow *row;
};

//TreeNode * tree_node_new  (char *name, char *path, Status status);
//void       tree_node_free (TreeNode *node);
//GNode *    tree_node_find (GNode *node,   char *name);
void		tree_node_add	(TreeNode *parent, char *orig_path, char **parts, Status status);
void		tree_print	(TreeNode *node, int depth);

TreeNode *	tree_parse_diff (FILE *file, gchar *left, gchar *right, GnomeApp *app);

#endif // _NODE_H_

