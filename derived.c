/*
    derived.c -- description
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

#include <gnome.h>
#include "derived.h"
#include "spawn.h"
#include "progress.h"
#include "global.h"
#include "node.h"
#include "tree.h"
#include "menu.h"
#include "mdi.h"

static gint (* old_button_handler)  (GtkWidget *widget, GdkEventButton *event) = NULL;
static gint (* old_key_handler)     (GtkWidget *widget, GdkEventKey    *event) = NULL;
static void (* old_draw_handler)    (GtkWidget *widget, GdkRectangle *area)    = NULL;
static void (* old_realize_handler) (GtkWidget *widget)                        = NULL;
static void (* old_show_handler)    (GtkWidget *widget)                        = NULL;

//XXX double click prototype needs to be:
//XXX int handler (GtkWidget *tree, TreeNode *node);

/*----------------------------------------------------------------------------*/
//static GtkStatusbar * get_view_statusbar(GtkDiffTree *tree);
static GtkWidget * gtk_diff_tree_new_with_titles (gint columns, gint tree_column, gchar *titles[], DiffOptions *diff);
//static char * dup_and_add_slash (char *path);
static gint gtk_diff_tree_button_press_event (GtkWidget *widget, GdkEventButton *event);
static gint gtk_diff_tree_key_press_event (GtkWidget *widget, GdkEventKey *event);
static gint tree_compare (GtkCList * clist, gconstpointer ptr1, gconstpointer ptr2);
static void gtk_diff_tree_class_init	 (GtkDiffTreeClass * klass);
static void gtk_diff_tree_compare(GtkDiffTree *tree, char *left, char *right);
static void gtk_diff_tree_draw (GtkWidget *widget, GdkRectangle *area);
static void gtk_diff_tree_destroy	 (GtkObject * object);
static void gtk_diff_tree_init		 (GtkDiffTree * diff_tree);
static void gtk_diff_tree_realize (GtkWidget *widget);
//static void gtk_diff_tree_set_view (GtkDiffTree *tree, Status status);
static void gtk_diff_tree_show (GtkWidget *widget);

GtkWidget *	gtk_diff_tree_new	(gint columns, gint tree_column, DiffOptions *diff);
guint		gtk_diff_tree_get_type	(void);
DiffOptions *	get_current_selection	(GtkDiffTree *tree);
void		gtk_diff_tree_save_list	(GtkDiffTree *tree);
/*----------------------------------------------------------------------------*/

//______________________________________________________________________________
//
// Public
//

guint
gtk_diff_tree_get_type (void)
{
	static guint diff_tree_type = 0;
	static const GtkTypeInfo diff_tree_info =
	{
		"GtkDiffTree",
		sizeof (GtkDiffTree),
		sizeof (GtkDiffTreeClass),
		 (GtkClassInitFunc)  gtk_diff_tree_class_init,
		 (GtkObjectInitFunc) gtk_diff_tree_init,
		/* reserved */ NULL,
		/* reserved */ NULL,
		 (GtkClassInitFunc) NULL,
	};

	//g_print ("gtk_diff_tree_get_type\n");
	if (!diff_tree_type)
	{
		// has to be 'derived' from ctree to be able to cast to it!
		diff_tree_type = gtk_type_unique (gtk_ctree_get_type(), &diff_tree_info);
	}

	return diff_tree_type;
}

GtkWidget *
gtk_diff_tree_new (gint columns, gint tree_column, DiffOptions *diff)
{
	//g_print ("gtk_diff_tree_new\n");
	return gtk_diff_tree_new_with_titles (columns, tree_column, NULL, diff);
}

static gint
tree_compare (GtkCList * clist, gconstpointer ptr1, gconstpointer ptr2)
{
	const GtkCTreeRow *row1 = ptr1;
	const GtkCTreeRow *row2 = ptr2;

	int leaf1 = (row1->is_leaf);
	int leaf2 = (row2->is_leaf);

	char *text1 = GTK_CELL_PIXTEXT (row1->row.cell[clist->sort_column])->text;
	char *text2 = GTK_CELL_PIXTEXT (row2->row.cell[clist->sort_column])->text;

	//XXX check options clist is our tree obj, we can get the options from there
	if (leaf1 != leaf2)							// One leaf and one node
	{
		return ((leaf1) ? 1 : -1);
	}
	else
	{
		return strcmp (text1, text2);
	}
}

static GtkWidget *
gtk_diff_tree_new_with_titles (gint columns, gint tree_column, gchar *titles[], DiffOptions *diff)
{
	GtkWidget *widget = NULL;
	GtkCList  *list	  = NULL;
	GtkCTree  *tree	  = NULL;
	GtkDiffTree *difftree = NULL;

	g_return_val_if_fail (columns     >  0,       NULL);
	g_return_val_if_fail (tree_column >= 0,       NULL);
	g_return_val_if_fail (tree_column <  columns, NULL);

	widget = gtk_widget_new (GTK_TYPE_DIFF_TREE,
				 "n_columns",   columns, //XXX always create max cols then hide xs
				 "tree_column", tree_column,
				 NULL);

	g_return_val_if_fail (widget != NULL, NULL);

	list = GTK_CLIST (widget);
	tree = GTK_CTREE (widget);
	difftree = GTK_DIFF_TREE (widget);

	list->columns     = columns;
	tree->tree_column = tree_column;
	difftree->diff = diff;

	if (titles)
	{
		int i;
		for (i = 0; i < columns; i++)
		{
			gtk_clist_set_column_title (list, i, titles[i]);
		}

		gtk_clist_column_titles_show (list);
	}

	// Nobody can be using these chunks, yet...
	g_mem_chunk_destroy (list->row_mem_chunk);
	list->row_mem_chunk = g_mem_chunk_create (DiffTreeRow, 1024, G_ALLOC_AND_FREE);

	gtk_clist_set_selection_mode     (list, GTK_SELECTION_BROWSE);
	gtk_clist_set_auto_sort          (list, TRUE);
	gtk_clist_set_compare_func       (list, tree_compare);
	gtk_clist_set_column_auto_resize (list, 0, TRUE);
	gtk_clist_column_titles_passive  (list);

	return widget;
}

DiffOptions *
get_current_selection (GtkDiffTree *tree)
{
	// XXX ask the tree for the selection -- it only allows 1!
	// XXX link the TreeNodes to the DataNodes and vice versa
	GtkCList	*clist		= NULL;
	GList		*selection	= NULL;
	GList		*list		= NULL;
	DiffTreeRow	*treerow	= NULL;
	DiffOptions	*options	= NULL;

	g_return_val_if_fail (tree != NULL, NULL);
	clist = GTK_CLIST (tree);

	g_return_val_if_fail (clist != NULL, NULL);
	selection = clist->selection;

	g_return_val_if_fail (selection != NULL, NULL);
	list = (selection->data);

	g_return_val_if_fail (list != NULL, NULL);
	treerow = (DiffTreeRow*) list->data;

	g_return_val_if_fail (treerow != NULL, NULL);
	options = diffoptions_new();

	g_return_val_if_fail (options != NULL, NULL);
	options->left  = g_strconcat (tree->left,  G_DIR_SEPARATOR_S, treerow->path, NULL);
	options->right = g_strconcat (tree->right, G_DIR_SEPARATOR_S, treerow->path, NULL);
	options->relative = g_strdup (treerow->path);
	options->left_root = g_strdup (tree->left);
	options->right_root = g_strdup (tree->right);
	options->type  = File;

	return options;
}

/*
static void
gtk_diff_tree_set_view (GtkDiffTree *tree, Status status)
{
	g_return_if_fail (tree != NULL);
	g_return_if_fail (status >= eFileSame);
	g_return_if_fail (status <= eFileAll);

	tree->view = status;
}
*/

/*
static char *
dup_and_add_slash (char *path)
{
	char *temp = NULL;
	int end = 0;

	g_return_val_if_fail (path != NULL, NULL);

	end = strlen (path) - 1;
	if ((end >= 0) && (path[end] == G_DIR_SEPARATOR))
	{
		temp = g_strdup (path);
	}
	else
	{
		temp = g_strconcat (path, G_DIR_SEPARATOR_S, NULL);
	}

	return temp;
}
*/

// TODO move this code out of the tree
// get someone else to do the spawn / parse stuff (MDI maybe?)
// tree_dialog_draw (tree, eFileAll);

/*
static GtkStatusbar *
get_view_statusbar(GtkDiffTree *tree)
{
	GnomeApp *app = NULL;

	g_return_val_if_fail (tree != NULL, NULL);

	app = gnome_mdi_get_app_from_view (GTK_WIDGET (tree));

	g_return_val_if_fail (app != NULL, NULL);

	return GTK_STATUSBAR (app->statusbar);
}
*/

static void
gtk_diff_tree_compare(GtkDiffTree *tree, char *left, char *right)
{
	FILE	 *file    = NULL;
	//Progress *progress = NULL;

	g_return_if_fail (tree  != NULL);
	g_return_if_fail (left  != NULL);
	g_return_if_fail (right != NULL);

	g_free (tree->left);
	g_free (tree->right);

	//tree->left  = dup_and_add_slash (left);
	//tree->right = dup_and_add_slash (right);
	tree->left  = g_strdup (left);
	tree->right = g_strdup (right);

	//progress = progress_new (get_view_statusbar(tree));
	//progress = progress_new (global_statusbar);

	// P -> no 'right' files! will need to stat left/right files
	//file = run_diff (g_strdup_printf ("diff -qrsP %s %s", tree->left, tree->right));
	file = run_diff (g_strdup_printf ("diff -qrs %s %s", tree->left, tree->right));

	g_return_if_fail (file);

	tree->root = tree_parse_diff (file, tree->left, tree->right, NULL);

	fclose (file);			// temp
	//tree_print (tree->root, 0);	// temp

	//progress_free (progress);
	tree_dialog_draw (tree, eFileAll);
}

//______________________________________________________________________________
//
// Private

static void
gtk_diff_tree_init (GtkDiffTree * tree)
{
	g_return_if_fail (tree != NULL);

	tree->left  = NULL;					// initialise values
	tree->right = NULL;
	tree->view  = eFileAll;
	tree->diff  = NULL;
	tree->drawn = FALSE;

	//g_print ("gtk_diff_tree_init\n");
}

static void
gtk_diff_tree_show (GtkWidget *widget)
{
	//g_print ("gtk_diff_tree_show\n");
	old_show_handler (widget);
}

static void
gtk_diff_tree_draw (GtkWidget *widget, GdkRectangle *area)
{
	GtkDiffTree *tree = GTK_DIFF_TREE (widget);

	//g_print ("gtk_diff_tree_draw\n");

	if (!tree->drawn)
	{
		tree->drawn = TRUE;
		gtk_diff_tree_compare (tree, tree->diff->left, tree->diff->right);
	}

	old_draw_handler (widget, area);
}

static void
gtk_diff_tree_realize (GtkWidget *widget)
{
	//g_print ("gtk_diff_tree_realize\n");
	old_realize_handler (widget);
}

static gint
gtk_diff_tree_key_press_event (GtkWidget *widget, GdkEventKey *event)
{
	//g_print ("key press\n");
	return old_key_handler (widget, event);
}

static gint
gtk_diff_tree_button_press_event (GtkWidget *widget, GdkEventButton *event)
{
	DiffOptions *diff = NULL;
	gboolean result = FALSE;
	GtkDiffTree *tree = NULL;
	GnomeMDIChild *child = NULL;
	GnomeMDI *mdi = NULL;

	g_return_val_if_fail (widget != NULL, FALSE);
	g_return_val_if_fail (GTK_IS_DIFF_TREE (widget), FALSE);

	tree = GTK_DIFF_TREE (widget);

	//g_print ("button %d\n", event->button);	// 1,2,3
	//g_print ("state %d\n", event->state);	// shift, ctrl, alt etc
	if (event->type == GDK_2BUTTON_PRESS)
	{
		// my handler
		g_print ("double click\n");
		diff = get_current_selection (tree);
		child = gnome_mdi_get_child_from_view (widget->parent); // not tree but scrolled window
		mdi = GNOME_MDI (child->parent);
		mdi_add_diff (mdi, diff);
		result = TRUE;
	}
	/*
	else if (event->type == GDK_3BUTTON_PRESS)
	{
		g_print ("triple click\n");
		result = TRUE;
	}
	else if (event->type == GDK_BUTTON_PRESS)
	{
		g_print ("single click\n");
		result 	= TRUE;
	}
	*/
	else
	{
		result = old_button_handler (widget, event);
	}

	return result;
}

static void
gtk_diff_tree_class_init (GtkDiffTreeClass * klass)
{
	GtkObjectClass *object_class = NULL;
	GtkWidgetClass *widget_class = NULL;

	g_return_if_fail (klass != NULL);

	//g_print ("gtk_diff_tree_class_init\n");

	// override methods
	object_class = (GtkObjectClass*) klass;
	object_class->destroy = gtk_diff_tree_destroy;

	widget_class = (GtkWidgetClass*) klass;

	old_button_handler = widget_class->button_press_event;
	old_key_handler    = widget_class->key_press_event;
	old_show_handler   = widget_class->show;
	old_realize_handler= widget_class->realize;
	old_draw_handler   = widget_class->draw;

	widget_class->button_press_event = gtk_diff_tree_button_press_event;
	widget_class->key_press_event    = gtk_diff_tree_key_press_event;
	widget_class->show               = gtk_diff_tree_show;
	widget_class->draw               = gtk_diff_tree_draw;
	widget_class->realize            = gtk_diff_tree_realize;
}

static void
gtk_diff_tree_destroy (GtkObject *object)
{
	GtkObjectClass	*parent = NULL;
	//GtkDiffTree	*diff_tree = GTK_DIFF_TREE (object);

	parent = gtk_type_class (gtk_ctree_get_type());		// my parent class

	g_return_if_fail (parent != NULL);

	parent->destroy (object);
}

static void
save_recurse (TreeNode *node)
{
	/*
	TreeNode *tree   = NULL;
	char      status = 0;

	while (node != NULL)
	{
		if (node->children)
		{
			save_recurse (node->children);	// it's a directory
		}
		else
		{
			tree = node->data;
			if (tree)
			{
				switch (tree->status)
				{
					case eFileSame:	 status = ' '; break;
					case eFileLeft:  status = '<'; break;
					case eFileRight: status = '>'; break;
					case eFileDiff:  status = '!'; break;
					case eFileType:  status = 'T'; break;
					case eFileError:
					default:         status = 'E'; break;
				}
				g_print ("%c %s\n", status, tree->path);
			}
		}

		node = node->next;
	}
	*/
}

void
gtk_diff_tree_save_list (GtkDiffTree *tree)
{
	// how about a heading (optional) diff linux-2.2.12 linux-2.3.18
	// What about the root pathname?
	save_recurse (tree->root);
}

