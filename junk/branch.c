#include <stdio.h>
#include <string.h>
#include "branch.h"
#include "leaf.h"
#include "icons/open.xpm"
#include "icons/closed.xpm"

CBranch::CBranch (GtkWidget *tree)
{
	pParent = NULL;
	pTree = tree;

	//m_pArray = g_ptr_array_new();
#if 0
	if (name)
	{
		m_pName  = g_string_new (name);
	}
#endif

	pixmap_open	= gdk_pixmap_create_from_xpm_d (pTree->window, &mask_open,   NULL, open_xpm);
	pixmap_closed	= gdk_pixmap_create_from_xpm_d (pTree->window, &mask_closed, NULL, closed_xpm);
}

CBranch::~CBranch()
{
#if 0
	if (m_pArray)
	{
		g_ptr_array_free (m_pArray, TRUE);
	}

	if (m_pName)
	{
		g_string_free (m_pName, TRUE);
		delete m_pName;
	}
#endif
}

void CBranch::SetName (char *name)
{
}

int CBranch::AddLeaf (char *dir, char *file)
{
	CLeaf *pLeaf = new CLeaf (pTree);

	g_ptr_array_add (m_pArray, pLeaf);
#if 0
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
#endif
}

gboolean CBranch::Initialise (GtkCTreeNode *parent, char *name)
{
	//gchar          *text[2];
	//gchar           buf1[60];
	//gchar           buf2[60];
	//gint		spacing = 5;

	//text[0] = buf1;
	//text[1] = buf2;

	//sprintf (buf1, "Branch %s", name);
	//sprintf (buf2, "Another %s", name);

	//GtkCTreeNode *parent_node = gtk_ctree_insert_node (GTK_CTREE (pTree), parent, NULL, text, spacing, pixmap_closed, mask_closed, pixmap_open, mask_open, FALSE, FALSE);

	//gtk_ctree_insert_node (GTK_CTREE (pTree), parent_node, NULL, text, spacing, pixmap_leaf, mask_leaf, NULL, NULL, TRUE, FALSE);
	//gtk_ctree_insert_node (GTK_CTREE (pTree), parent_node, NULL, text, spacing, pixmap_leaf, mask_leaf, NULL, NULL, TRUE, FALSE);

	pParent= parent ;
	m_pName = g_string_new (name);

	//g_print ("CBranch::Initialise name = ");
	//g_print (m_pName->str);
	//g_print ("\n");

	char *p = strchr (name, '/');
	if (p)
	{
		GString *remainder = g_string_new (p+1);

		int len = ((long) p) - ((long) name);

		g_string_truncate (m_pName, len);

		g_print ("CBranch::Initialise (");
		g_print (m_pName->str);
		g_print (")\n");

		CBranch *n = new CBranch (pTree);
		n->Initialise (NULL, remainder->str);
	}
	else
	{
		//new leaf
		CLeaf *leaf = new CLeaf (pTree);
		leaf->Initialise (NULL, name);

	}

	return TRUE;
}

