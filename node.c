#include <stddef.h>
#include <string.h>
#include <gtk/gtk.h>
#include "node.h"
#if 0
#include "icons/open.xpm"
#include "icons/closed.xpm"
#include "icons/leaf.xpm"

static GdkPixmap *g_PixmapOpen		= NULL;
static GdkPixmap *g_PixmapClosed	= NULL;
static GdkPixmap *g_PixmapLeaf		= NULL;

static GdkBitmap *g_MaskOpen		= NULL;
static GdkBitmap *g_MaskClosed		= NULL;
static GdkBitmap *g_MaskLeaf		= NULL;

CNode::CNode (GtkWidget *pTree)
{
	m_pTree    = pTree;
	m_pParent  = NULL;
	m_pNode    = NULL;
	m_pSibling = NULL;

	g_return_if_fail (m_pTree);

	if (g_PixmapOpen == NULL)
	{
		g_PixmapOpen	= gdk_pixmap_create_from_xpm_d (m_pTree->window, &g_MaskOpen,   NULL, open_xpm);
		g_PixmapClosed	= gdk_pixmap_create_from_xpm_d (m_pTree->window, &g_MaskClosed, NULL, closed_xpm);
		g_PixmapLeaf	= gdk_pixmap_create_from_xpm_d (m_pTree->window, &g_MaskLeaf,   NULL, leaf_xpm);
	}

	m_pBranches = g_ptr_array_new();
	m_pNames    = g_ptr_array_new();
}

CNode::~CNode()
{
}

CNode * CNode::FindNode (char *pName)
{
	GString *pStr = NULL;

	int iSize = m_pNames->len;

	//g_print ("size = %d, %p\n", m_pNames->len, pName);

	for (int i = 0; i < iSize; i++)
	{
		pStr = (GString*) g_ptr_array_index (m_pNames, i);

		//g_print ("item %d = %s\n", i, pStr->str);
		if (strcmp (pName, pStr->str) == 0)
		{
			return (CNode*) g_ptr_array_index (m_pBranches, i);
		}
	}
	return NULL;
}

GtkCTreeNode * CNode::AddNode (GtkCTreeNode *pParent, GtkCTreeNode *pSibling, char *pName, Status eStatus)
{
	g_return_val_if_fail (m_pTree, NULL);
	g_return_val_if_fail (pName,   NULL);

	int	  iSpacing = 5;
	char	 *pLabel   = NULL;
	char	 *pNoLabel = "";
	GdkColor  DirColor;
	GdkColor  FileColor;
	GtkStyle *pDirStyle  = gtk_style_new ();
	GtkStyle *pFileStyle = gtk_style_new ();
	GtkStyle *pNodeStyle = NULL;

	memset (&DirColor,  0, sizeof (DirColor));
	memset (&FileColor, 0, sizeof (FileColor));

	DirColor.red   = 65535;
	DirColor.green =     0;
	DirColor.blue  =     0;

	FileColor.red   = (eStatus == eFileDiff)  ? 65535 : 0;
	FileColor.green = (eStatus == eFileLeft)  ? 65535 : 0;
	FileColor.blue  = (eStatus == eFileRight) ? 65535 : 0;

	pDirStyle ->fg[GTK_STATE_NORMAL] = DirColor;
	pFileStyle->fg[GTK_STATE_NORMAL] = FileColor;

	switch (eStatus)
	{
		case eFileSame:  pLabel = pNoLabel;  break;
		case eFileLeft:  pLabel = "Left";    break;
		case eFileRight: pLabel = "Right";   break;
		case eFileDiff:  pLabel = "Diff";    break;
		case eFileType:  pLabel = "Type";    break;
		default:         pLabel = "Error";   break;
	}

	char *pSlash = strchr (pName, '/');
	if (pSlash)
	{
		GString *pRemainder = g_string_new (pSlash+1);
		GString *pBaseName  = g_string_new (pName);

		int len = pSlash - pName;			//int len = ((long) pSlash) - ((long) pName);

		g_string_truncate (pBaseName, len);

		CNode *pExists = FindNode (pBaseName->str);
		if (pExists != NULL)
		{

			pExists->AddNode (pExists->m_pParent, pExists->m_pSibling, pRemainder->str, eStatus);
		}
		else
		{
			m_pParent = pParent;

			char *text[2] = { pBaseName->str, pNoLabel };

			// NULL was pSibling (insert = insert before)
			m_pSibling = gtk_ctree_insert_node (GTK_CTREE (m_pTree), m_pParent, pSibling, text, iSpacing, g_PixmapClosed, g_MaskClosed, g_PixmapOpen, g_MaskOpen, FALSE, FALSE);

			CNode *pNewNode = new CNode (m_pTree);
			pNewNode->AddNode (m_pSibling, NULL, pRemainder->str, eStatus);

			g_ptr_array_add (m_pBranches, pNewNode);
			g_ptr_array_add (m_pNames,    pBaseName);

		}

		pNodeStyle = pDirStyle;
	}
	else
	{
		char *text[2] = { pName, pLabel };

		m_pParent  = pParent;
		pNodeStyle = pFileStyle;

		// NULL was pSibling (insert = insert before)
		m_pSibling = gtk_ctree_insert_node (GTK_CTREE (m_pTree), m_pParent, pSibling, text, iSpacing, g_PixmapLeaf, g_MaskLeaf, NULL, NULL, TRUE, FALSE);
	}

	if (eStatus != eFileSame)
	{
		gtk_ctree_node_set_cell_style (GTK_CTREE (m_pTree), m_pSibling, 0, pNodeStyle);
	}

	return pSibling;
}

#endif

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
