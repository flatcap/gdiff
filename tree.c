/*
    tree.c -- description
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

/* $Revision: 1.17 $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gnome.h>
#include "tree.h"
#include "node.h"
#include "global.h"

/*----------------------------------------------------------------------------*/
//static GtkStyle * get_status_style (Status status, gboolean node);
//static char * get_status_text (Status actual, Status view, gboolean node);
//static void set_row_data (GList *list, TreeNode *data);
static void tree_dialog_traverse (GtkDiffTree *tree, GtkCTreeNode *parent, TreeNode *node, Status status);

gboolean tree_dialog_draw (GtkDiffTree *tree, Status status);
/*----------------------------------------------------------------------------*/

#if 0
static char *
get_status_text (Status actual, Status view, gboolean node)
{
	char *result = "";

	if (node)
	{
		GString *s = g_string_new ("");

		if (actual & eFileLeft  & view) g_string_append (s, "left ");
		if (actual & eFileRight & view) g_string_append (s, "right ");
		if (actual & eFileDiff  & view) g_string_append (s, "diff ");
		if (actual & eFileSame  & view) g_string_append (s, "");
		if (actual & eFileType  & view) g_string_append (s, "type ");
		if (actual & eFileError & view) g_string_append (s, "error ");

		result = s->str;
	}
	else
	{
		switch (actual)
		{
			case eFileSame:  result = "";		break;
			case eFileLeft:  result = "left";	break;
			case eFileRight: result = "right";	break;
			case eFileDiff:  result = "diff";	break;
			case eFileType:  result = "type";	break;
			default:         result = "error";	break;
		}
	}

	return g_strdup (result);
}

static GtkStyle *
get_status_style (Status status, gboolean node)
{
	GtkStyle *style = gtk_style_new ();
	GdkColor  fg    = { 0, 0, 0, 0 };

	fg.red   = (status & eFileDiff)  ? 65535 : 0;
	fg.green = (status & eFileLeft)  ? 65535 : 0;
	fg.blue  = (status & eFileRight) ? 65535 : 0;

	if (!node)//temp
	{
		style->fg[GTK_STATE_NORMAL] = fg;
	}

	/*
	switch (status)
	{
		case eFileSame:		return "";
		case eFileLeft:		return "left";
		case eFileRight:	return "right";
		case eFileDiff:		return "diff";
		case eFileType:		return "type";
		default:		return "error";
	}
	*/

	return style;
}

static void
set_row_data (GList *list, TreeNode *data)	// list is base class of GtkCTreeNode
{
	DiffTreeRow *diffrow = list->data;

	diffrow->name   = g_strdup (data->name);
	diffrow->path   = g_strdup (data->path);
	diffrow->status = data->status;
}
#endif

static void
tree_dialog_traverse (GtkDiffTree *tree, GtkCTreeNode *parent, TreeNode *node, Status status)
{
	/*
	//GtkCTreeNode *new_node = NULL;
	GtkCTreeNode *sibling  = NULL;
	TreeNode     *tnode    = NULL;
	char         *text[2]  = { NULL, NULL };

	g_return_if_fail (tree);
	g_return_if_fail (node);
	//g_return_if_fail (node->data);

	while (node)
	{
		tnode = (TreeNode*) node->data;
		if (tnode)
		{
			text[0] = tnode->name;
			//g_print ("%s (children %d)\n", text[1], (node->children != NULL));
			if (node->children)
			{
				if (tnode->status & status)
				{
					text[1] = get_status_text (tnode->status, status, TRUE);
					//g_print ("Node1: %s, %s\n", text[0], text[1]);
					sibling = gtk_ctree_insert_node (GTK_CTREE (tree), parent, sibling, text, 5, pixmap_closed, mask_closed, pixmap_open, mask_open, FALSE, FALSE);
					set_row_data ((GList*) sibling, tnode);
				}
			}
			else
			{
				if (status & tnode->status)
				{
					text[1] = get_status_text (tnode->status, status, FALSE);
					//g_print ("Node2: %s, %s\n", text[0], text[1]);
					sibling = gtk_ctree_insert_node (GTK_CTREE (tree), parent, sibling, text, 5, pixmap_leaf, mask_leaf, NULL, NULL, TRUE, FALSE);
					set_row_data ((GList*) sibling, tnode);
					g_free (text[1]);
					//g_print ("%s (%d) %p\n", tnode->name, tnode->status, sibling);
				}
			}

			if ((sibling) && (status & tnode->status))
			//if (sibling)
			{ 
				GtkStyle *style = get_status_style (tnode->status, (node->children != NULL));
				gtk_ctree_node_set_cell_style (GTK_CTREE (tree), sibling, 0, style);
				gtk_style_unref (style);
				//g_print ("get_status_style (%d, %d);\n", tnode->status, (node->children != NULL));
			}
		}

		if (node->children)
		{
			tree_dialog_traverse (tree, sibling, node->children, status);
		}

		node = node->next;
	}
	*/
}

gboolean 
tree_dialog_draw (GtkDiffTree *tree, Status status)
{
	g_return_val_if_fail (tree,         FALSE);
	//g_return_val_if_fail (tree->dialog, FALSE);
	//g_return_val_if_fail (tree->ctree,  FALSE);

	tree->view = status;

	//g_print ("max height = %d\n", g_node_max_height (tree->root));
	//g_print ("root = %p\n", tree->root);
	//g_print ("children = %p\n", tree->root->children);
	//g_print ("prev = %p\n", tree->root->prev);
	//g_print ("next = %p\n", tree->root->next);

	//gtk_widget_show (tree->dialog);
	//gtk_widget_show (tree->ctree);

	//tree_print (tree->root, 0);

	gtk_clist_freeze     (GTK_CLIST (tree));
	gtk_clist_clear      (GTK_CLIST (tree));
	tree_dialog_traverse (GTK_DIFF_TREE (tree), NULL, (TreeNode*) tree->root->gnode.children, status);
	gtk_clist_thaw       (GTK_CLIST (tree));

	return TRUE;
}

