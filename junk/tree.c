#include <stdio.h>
#include "tree.h"
#include "branch.h"
#include "icons/open.xpm"
#include "icons/closed.xpm"
#include "icons/leaf.xpm"

char *column_headings[] = {"Diff?",  "Name"};

CTree::CTree (GtkWidget *window)
{
	g_return_if_fail (window);

	parent		= window;
	tree		= NULL;
	mask_open	= NULL;
	mask_closed	= NULL;
	mask_leaf	= NULL;
	pixmap_open	= NULL;
	pixmap_closed	= NULL;
	pixmap_leaf	= NULL;

	gtk_widget_show  (parent);
	g_return_if_fail (parent->window);

	pixmap_open	= gdk_pixmap_create_from_xpm_d (parent->window, &mask_open,   NULL, open_xpm);
	pixmap_closed	= gdk_pixmap_create_from_xpm_d (parent->window, &mask_closed, NULL, closed_xpm);
	pixmap_leaf	= gdk_pixmap_create_from_xpm_d (parent->window, &mask_leaf,   NULL, leaf_xpm);

	g_return_if_fail (pixmap_open && pixmap_closed && pixmap_leaf);
	
	tree = gtk_ctree_new_with_titles (2, 0, column_headings);

	g_return_if_fail (tree);

	gtk_clist_set_column_auto_resize (GTK_CLIST (tree), 0, TRUE);

#if 0
	gtk_clist_set_row_height (GTK_CLIST (tree), 40);
	gtk_clist_set_reorderable (GTK_CLIST (tree), TRUE);
	gtk_clist_set_column_width (GTK_CLIST (tree), 1, 200);
	void gtk_clist_set_column_min_width (GtkCList *clist,
					     gint      column,
					     gint      min_width);
	void gtk_clist_set_column_max_width (GtkCList *clist,
					     gint      column,
					     gint      max_width);
#endif

	gtk_container_add (GTK_CONTAINER (window), tree);
	gtk_widget_show (tree);
}

CTree::~CTree()
{
	delete pixmap_open;
	delete pixmap_closed;
	delete pixmap_leaf;
	delete mask_open;
	delete mask_closed;
	delete mask_leaf;
}

gboolean CTree::AddBranch (char *branch)
{
#if 0
	gchar          *text[2];
	gchar           buf1[60];
	gchar           buf2[60];
	gint		spacing = 5;

	text[0] = buf1;
	text[1] = buf2;

	sprintf (buf1, "Branch %s", branch);
	sprintf (buf2, "Another %s", branch);

	GtkCTreeNode *sibling = gtk_ctree_insert_node (GTK_CTREE (tree), NULL, NULL, text, spacing, pixmap_closed, mask_closed, pixmap_open, mask_open, FALSE, FALSE);

	gtk_ctree_insert_node (GTK_CTREE (tree), sibling, NULL, text, spacing, pixmap_leaf, mask_leaf, NULL, NULL, TRUE, FALSE);
	gtk_ctree_insert_node (GTK_CTREE (tree), sibling, NULL, text, spacing, pixmap_leaf, mask_leaf, NULL, NULL, TRUE, FALSE);
#endif
	g_print ("tree (");
	g_print (branch);
	g_print (")\n");

	CBranch *pBranch = new CBranch (tree);
	pBranch->Initialise (NULL, branch);

	return TRUE;
}

gboolean CTree::AddLeaf (char *leaf)
{
	gchar          *text[2];
	gchar           buf1[60];
	gchar           buf2[60];
	gint		spacing = 5;

	text[0] = buf1;
	text[1] = buf2;

	sprintf (buf1, "Leaf %s", leaf);
	sprintf (buf2, "Another %s", leaf);

	GtkCTreeNode *sibling = gtk_ctree_insert_node (GTK_CTREE (tree), NULL, NULL, text, spacing, pixmap_leaf, mask_leaf, NULL, NULL, TRUE, FALSE);

	return TRUE;
}

