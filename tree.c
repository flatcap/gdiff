/* $Revision$ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gnome.h>
#include "tree.h"
#include "node.h"
#include "global.h"

// Local methods
Status tree_dialog_parse_diff_line (GtkDiffTree *tree, char *buffer, GString *path);

FILE *
tree_dialog_start_diff (GtkDiffTree *tree, char *options)
{
	return stdin;
}

//temp
void tree_print (GNode *node, int depth);

char *
get_status_text (Status actual, Status view, gboolean node)
{
	char *result = "";

	if (node)
	{
		GString *s = g_string_new ("");

		//g_print ("left = %d\t", actual & eFileLeft);
		if (actual & eFileLeft & view)  g_string_append (s, "left ");
		//g_print ("right = %d\t", actual & eFileRight);
		if (actual & eFileRight & view) g_string_append (s, "right ");
		//g_print ("diff = %d\t", actual & eFileDiff);
		if (actual & eFileDiff & view)  g_string_append (s, "diff ");
		//g_print ("same = %d\t", actual & eFileSame);
		if (actual & eFileSame & view)  g_string_append (s, "");
		//g_print ("type = %d\t", actual & eFileType);
		if (actual & eFileType & view)  g_string_append (s, "type ");
		//g_print ("error = %d\t", actual & eFileError);
		if (actual & eFileError & view) g_string_append (s, "error ");
		//g_print ("'%s'\n", s->str);

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

GtkStyle *
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

void
set_row_data (GList *list, TreeNode *data)	// list is base class of GtkCTreeNode
{
	DiffTreeRow *diffrow = list->data;
	//GtkCTreeRow *treerow = list->data;
	//GtkCListRow *listrow = list->data;

	diffrow->name   = g_strdup (data->name);
	diffrow->path   = g_strdup (data->path);
	diffrow->status = data->status;

	//g_print ("row = %p\n", list->data);
	/*
	g_print ("%p\n", list);
	g_print ("\tcell = %p, ", listrow->cell);
	g_print ("state = %d, ", listrow->state);
	//g_print ("fg = %ld, %d, %d, %d, ", listrow->foreground.pixel, listrow->foreground.red, listrow->foreground.green, listrow->foreground.blue);
	//g_print ("bg = %ld, %d, %d, %d, ", listrow->background.pixel, listrow->background.red, listrow->background.green, listrow->background.blue);
	g_print ("style = %p, ", listrow->style);
	g_print ("data = %p, ", listrow->data);
	g_print ("notify = %p, ", listrow->destroy);
	//g_print ("fg_set = %d, ", listrow->fg_set);
	//g_print ("bg_set = %d, ", listrow->bg_set);
	//g_print ("selectable = %d\n", listrow->selectable);
	g_print ("\n");

	g_print ("\tparent = %p, ", treerow->parent);
	g_print ("sibling = %p, ", treerow->sibling);
	g_print ("children = %p, ", treerow->children);
	//g_print ("pixmap_closed = %p, ", treerow->pixmap_closed);
	//g_print ("mask_closed = %p, ", treerow->mask_closed);
	//g_print ("pixmap_opened = %p, ", treerow->pixmap_opened);
	//g_print ("mask_opened = %p, ", treerow->mask_opened);
	g_print ("level = %d, ", treerow->level);
	g_print ("is_leaf = %d, ", treerow->is_leaf);
	g_print ("expanded = %d\n", treerow->expanded);

	g_print ("\tname = %s, ", diffrow->name);
	g_print ("status = %d\n", diffrow->status);
	*/
}

void
tree_dialog_traverse (GtkDiffTree *tree, GtkCTreeNode *parent, GNode *node, Status status)
{
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
}

gboolean 
tree_dialog_redraw (GtkDiffTree *tree, Status status, gboolean add)
{
	// This method will maintain the tree expansion
	// freeze
	// if add check not there
	// if remove find and remove
	// thaw
	return FALSE;
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
	tree_dialog_traverse (GTK_DIFF_TREE (tree), NULL, tree->root->children, status);
	gtk_clist_thaw       (GTK_CLIST (tree));

	return TRUE;
}

void 
tree_dialog_free (GtkDiffTree *tree)
{
	g_free (tree->left);
	g_free (tree->right);

	//node_free (tree->root);

	g_free (tree);
}


