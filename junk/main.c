#include <iostream.h>
#include <string.h>
#include <regex.h>
#include <gtk/gtk.h>
#include "node.h"

#define MATCHES  8

const char *diff = "Files \\(.*\\)/\\(.*\\) and \\(.*\\)/\\2 differ";
const char *same = "Files \\(.*\\)/\\(.*\\) and \\(.*\\)/\\2 are identical";
const char *only = "Only in \\(.*\\): \\(.*\\)";
const char *type = "File \\(.*\\)/\\(.*\\) is a \\(.*\\) while file \\(.*\\)/\\2 is a \\(.*\\)";

#if 0
	"regular empty file"
	"regular file "
	"directory"
	"block special file"
	"character special file"
	"fifo "
	"message queue"
	"semaphore"
	"shared memory object"
	"socket"
	"weird file"

	"Only in %s: %s\n"
	"File %s is a %s while file %s is a %s\n"
	"Files %s and %s differ\n"
	"Files %s and %s are identical\n"
#endif

char *base_left  = NULL;
char *base_right = NULL;

void destroy (GtkWidget *pWidget, gpointer *pData)
{
	gtk_main_quit();
}

Status ParsePathName (char buffer[], GString *pPath)
{
	regex_t RegSame;				// These could be global
	regex_t RegDiff;
	regex_t RegOnly;
	regex_t RegType;

	g_return_val_if_fail ((regcomp (&RegSame, same, 0) == 0), eFileError);
	g_return_val_if_fail ((regcomp (&RegDiff, diff, 0) == 0), eFileError);
	g_return_val_if_fail ((regcomp (&RegOnly, only, 0) == 0), eFileError);
	g_return_val_if_fail ((regcomp (&RegType, type, 0) == 0), eFileError);

	regmatch_t matches[MATCHES];
	Status result = eFileError;

	if (regexec (&RegSame, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileSame;
	}
	else if (regexec (&RegDiff, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileDiff;
	}
	else if (regexec (&RegOnly, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileLeft;					// This may be overridden
	}
	else if (regexec (&RegType, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileType;
	}
	else
	{
		result = eFileError;
	}

	regfree (&RegSame);
	regfree (&RegDiff);
	regfree (&RegOnly);
	regfree (&RegType);

	if (result != eFileError)
	{
		g_string_assign   (pPath, buffer + matches[1].rm_so);
		g_string_truncate (pPath, matches[1].rm_eo - matches[1].rm_so);
		g_string_append   (pPath, "/");

		GString *pFile = g_string_new (buffer + matches[2].rm_so);
		g_string_truncate (pFile, matches[2].rm_eo - matches[2].rm_so);
		g_string_append (pPath, pFile->str);
		g_string_free (pFile, TRUE);

		int iLeft  = strlen (base_left);			// Should be global
		int iRight = strlen (base_right);

		if (strncmp (base_left, pPath->str, iLeft) == 0)
		{
			g_string_erase (pPath, 0, iLeft);		// Same, diff, type, and left only
		}
		else if (strncmp (base_right, pPath->str, iRight) == 0)
		{
			g_string_erase (pPath, 0, iRight);
			result = eFileRight;				// Right-only
		}
		else
		{
			result = eFileError;
		}
	}

	return result;
}

gint TreeCompare (GtkCList * clist, gconstpointer ptr1, gconstpointer ptr2)
{
	GtkCTreeRow *row1 = (GtkCTreeRow *) ptr1;
	GtkCTreeRow *row2 = (GtkCTreeRow *) ptr2;

	int leaf1 = (row1->is_leaf);
	int leaf2 = (row2->is_leaf);

	char *text1 = GTK_CELL_PIXTEXT (row1->row.cell[clist->sort_column])->text;
	char *text2 = GTK_CELL_PIXTEXT (row2->row.cell[clist->sort_column])->text;

	if (leaf1 != leaf2)				// One leaf and one node
	{
		return ((leaf1) ? 1 : -1);
	}
	else
	{
		return strcmp (text1, text2);
	}
}

int main (int argc, char *argv[], char *env[])
{
	gtk_init (&argc, &argv);

	if (argc != 3)
	{
		g_print ("Usage: prog left right\n");
		return 1;
	}

	base_left  = argv[1];
	base_right = argv[2];

	//g_print ("left = %s, right = %s\n", base_left, base_right);

	GtkWidget *pWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (pWindow), 500, 300);
	gtk_signal_connect (GTK_OBJECT (pWindow), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
	gtk_widget_show (pWindow);

	GtkWidget *pScrolled = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (pScrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER(pWindow), pScrolled);

	char *cols[] = {"Name", "Description"};
	gtk_widget_show (pScrolled);
	GtkWidget *pTree = gtk_ctree_new_with_titles (2, 0, cols);
	gtk_container_add (GTK_CONTAINER (pScrolled), pTree);
	gtk_widget_show (pTree);

	//gtk_ctree_set_line_style (GTK_CTREE (pTree), GTK_CTREE_LINES_TABBED);
	gtk_clist_set_selection_mode     (GTK_CLIST (pTree), GTK_SELECTION_BROWSE);
	gtk_clist_set_auto_sort          (GTK_CLIST (pTree), TRUE);
	gtk_clist_set_compare_func       (GTK_CLIST (pTree), TreeCompare);
	gtk_clist_set_column_auto_resize (GTK_CLIST (pTree), 0, TRUE);

	CNode *pNode = new CNode (pTree);

	GtkCTreeNode *pSibling = NULL;

	char buffer[256];
	while (TRUE)
	{
		cin.getline (buffer, sizeof (buffer));
		if (cin.eof())
		{
			break;
		}

		GString *pPath = g_string_new ("");
		Status eStatus = ParsePathName (buffer, pPath);

		//g_print ("status = %d\n", eStatus);
		pNode->AddNode (NULL, pSibling, pPath->str, eStatus);
	}

	//gtk_widget_show (pWindow);
	//gtk_widget_show (pScrolled);
	//gtk_widget_show (pTree);
	gtk_main();
	delete pNode;

	return 0;
}

