#include <stddef.h>
#include <string.h>
#include <gtk/gtk.h>
#include "node.h"
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
	m_pTree   = pTree;
	m_pParent = NULL;
	m_pNode   = NULL;
	m_pName   = NULL;

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

	//g_print ("size = %d\n", m_pNames->len);

	int iSize = m_pNames->len;

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

gboolean CNode::AddNode (GtkCTreeNode *pParent, char *pName, Status eStatus)
{
	g_return_val_if_fail (m_pTree, FALSE);
	g_return_val_if_fail (pName,   FALSE);

	//g_print ("%s - %p\n", pName, pParent);

	char *pSlash = strchr (pName, '/');
	if (pSlash)
	{
		GString *pRemainder = g_string_new (pSlash+1);
		GString *pBaseName  = g_string_new (pName);

		int len = pSlash - pName;			//int len = ((long) pSlash) - ((long) pName);

		g_string_truncate (pBaseName, len);

		//g_print ("searching for %s\n", pBaseName->str);

		CNode *pExists = FindNode (pBaseName->str);
		if (pExists != NULL)
		{
			//g_print ("found: %s - %p\n", pBaseName->str, pExists->m_pParent);

			pExists->AddNode (pExists->m_pParent, pRemainder->str, eStatus);
			if (eStatus != eFileSame)
			{
				GdkColor col1;
				col1.red   = 65535;
				col1.green =     0;
				col1.blue  =     0;

				GtkStyle *pStyle = gtk_style_new ();
				pStyle->fg[GTK_STATE_NORMAL] = col1;

				gtk_ctree_node_set_cell_style (GTK_CTREE (m_pTree), pExists->m_pParent, 0, pStyle);
			}
		}
		else
		{
			//g_print ("branch: %s\n", pBaseName->str);

			m_pName   = g_string_new (pBaseName->str);
			m_pParent = pParent;

			char *text[2];
			text[0] = m_pName->str;
			switch (eStatus)
			{
				case eFileSame:  text[1] = "Same";    break;
				case eFileLeft:  text[1] = "Left";    break;
				case eFileRight: text[1] = "Right";   break;
				case eFileDiff:  text[1] = "Diff";    break;
				default:         text[1] = "Unknown"; break;
			}
			text[1] = "";//m_pName->str;
			int spacing = 5;

			GtkCTreeNode *pBranch = gtk_ctree_insert_node (GTK_CTREE (m_pTree), pParent, NULL, text, spacing, g_PixmapClosed, g_MaskClosed, g_PixmapOpen, g_MaskOpen, FALSE, FALSE);

			CNode *pNewNode = new CNode (m_pTree);
			pNewNode->AddNode (pBranch, pRemainder->str, eStatus);

			//g_print ("Adding %s - %p\n", m_pName->str, pNewNode->m_pParent);

			g_ptr_array_add (m_pBranches, pNewNode);
			g_ptr_array_add (m_pNames,    m_pName);

			if (eStatus != eFileSame)
			{
				GdkColor col1;
				col1.red   = 65535;
				col1.green =     0;
				col1.blue  =     0;

				GtkStyle *pStyle = gtk_style_new ();
				pStyle->fg[GTK_STATE_NORMAL] = col1;

				gtk_ctree_node_set_cell_style (GTK_CTREE (m_pTree), pBranch, 0, pStyle);
			}
		}
	}
	else
	{
		//g_print ("leaf: %s\n", pName);

		m_pParent = pParent;

		char *text[2];
		text[0] = pName;
		switch (eStatus)
		{
			case eFileSame:  text[1] = "";        break;	// "Same"
			case eFileLeft:  text[1] = "Left";    break;
			case eFileRight: text[1] = "Right";   break;
			case eFileDiff:  text[1] = "Diff";    break;
			default:         text[1] = "Unknown"; break;
		}
		int spacing = 5;
		GtkCTreeNode *sibling = gtk_ctree_insert_node (GTK_CTREE (m_pTree), pParent, NULL, text, spacing, g_PixmapLeaf, g_MaskLeaf, NULL, NULL, TRUE, FALSE);

		if (eStatus != eFileSame)
		{
			GdkColor col1;
			col1.red   = (eStatus == eFileDiff)  ? 65535 : 0;
			col1.green = (eStatus == eFileLeft)  ? 65535 : 0;
			col1.blue  = (eStatus == eFileRight) ? 65535 : 0;

			GtkStyle *pStyle = gtk_style_new ();
			//pStyle->base[GTK_STATE_NORMAL] = col1;
			pStyle->fg[GTK_STATE_NORMAL] = col1;
#if 0
			GTK_STATE_NORMAL,
			GTK_STATE_ACTIVE,
			GTK_STATE_PRELIGHT,
			GTK_STATE_SELECTED,
			GTK_STATE_INSENSITIVE
#endif

			gtk_ctree_node_set_cell_style (GTK_CTREE (m_pTree), sibling, 0, pStyle);
		}

	}

	return TRUE;
}

