#include "leaf.h"

CLeaf::CLeaf (GtkWidget *pTree)
{
}

CLeaf::~CLeaf()
{
}

gboolean CLeaf::Initialise (GtkCTreeNode *pParent, char *name)
{
	g_print ("CLeaf::Initialise (");
	g_print (name);
	g_print (")\n");

}
