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

/* $Revision: 1.9 $ */

#ifndef _NODE_H_
#define _NODE_H_

#include <gtk/gtk.h>
#include "derived.h"

typedef struct _TreeNode TreeNode;

struct _TreeNode
{
	GNode   node;	//XXX FUTURE

	char   *name;
	char   *path;
	Status  status;
};

//TreeNode * tree_node_new  (char *name, char *path, Status status);
//void       tree_node_free (TreeNode *node);
//GNode *    tree_node_find (GNode *node,   char *name);
void		tree_node_add  (GNode *parent, char *path, Status status, char *orig_path);
void		tree_print     (GNode *node, int depth);

//XXX FUTURE
gboolean	tree_parse_diff (FILE *file, GnomeApp *app);

#endif // _NODE_H_

