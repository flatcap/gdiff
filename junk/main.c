#include <iostream.h>
#include <string.h>
#include <regex.h>
#include <gtk/gtk.h>
#include "node.h"

#define MATCHES 8
#define APPNAME "TestApp"
#define VERSION "0.0.1"

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

static gboolean bAbort = FALSE;

void destroy (GtkWidget *pWidget, gpointer *pData)
{
	gtk_main_quit();
}

void abort (GtkWidget *pWidget, gpointer *pData)
{
	bAbort = TRUE;
}

Status ParsePathName (char buffer[], GString *path)
{
	g_return_val_if_fail (buffer, eFileError);
	g_return_val_if_fail (path,   eFileError);

	regex_t RegSame;				// These should be global
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

	if (result == eFileError)
	{
		g_string_truncate (path, 0);
	}
	else
	{
		g_string_assign   (path, buffer + matches[1].rm_so);
		g_string_truncate (path, matches[1].rm_eo - matches[1].rm_so);
		g_string_append   (path, G_DIR_SEPARATOR_S);

		GString *file = g_string_new (buffer + matches[2].rm_so);
		g_string_truncate (file, matches[2].rm_eo - matches[2].rm_so);
		g_string_append (path, file->str);
		g_string_free (file, TRUE);

		int l = strlen (base_left);			// Should be global
		int r = strlen (base_right);

		if (strncmp (base_left, path->str, l) == 0)
		{
			g_string_erase (path, 0, l);		// Same, diff, type, and left only
		}
		else if (strncmp (base_right, path->str, r) == 0)
		{
			g_string_erase (path, 0, r);
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

void basename (GString *path)
{
	char *slash = strrchr (path->str, '/');

	if (slash)
	{
		g_string_truncate (path, (slash - path->str));
	}
}

int main (int argc, char *argv[], char *env[])
{
	//gnome_init (APPNAME, VERSION, argc, argv);

	if (argc != 3)
	{
		g_print ("Usage: prog left right\n");
		return 1;
	}

	base_left  = argv[1];
	base_right = argv[2];

	//g_print ("left = %s, right = %s\n", base_left, base_right);

#if 0
	GtkWidget *pWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (pWindow), 500, 50);
	gtk_signal_connect (GTK_OBJECT (pWindow), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
	gtk_widget_show (pWindow);

	GtkWidget *box = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (pWindow), box);
	gtk_widget_show (box);

	GtkWidget *label = gtk_label_new ("Processing...");
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 10);
	gtk_widget_show (label);

	GtkWidget *button = gtk_button_new_with_label ("Abort");
	gtk_box_pack_end (GTK_BOX (box), button, FALSE, FALSE, 10);
	//GtkAllocation a = { -1, -1, 20, 20 };
	//gtk_widget_size_allocate (button, &a);
	gtk_widget_show (button);
	//button->allocation.width  = 20;
	//button->allocation.height = 20;
	gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (abort), NULL);

	//GtkWidget *pScrolled = gtk_scrolled_window_new (NULL, NULL);
	//gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (pScrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	//gtk_container_add (GTK_CONTAINER(pWindow), pScrolled);

	//XX char *cols[] = {"Name", "Description"};
	//XX gtk_widget_show (pScrolled);
	//XX GtkWidget *pTree = gtk_ctree_new_with_titles (2, 0, cols);
	//XX gtk_container_add (GTK_CONTAINER (pScrolled), pTree);
	//XX gtk_widget_show (pTree);

	//gtk_ctree_set_line_style (GTK_CTREE (pTree), GTK_CTREE_LINES_TABBED);
	//XX gtk_clist_set_selection_mode     (GTK_CLIST (pTree), GTK_SELECTION_BROWSE);
	//XX gtk_clist_set_auto_sort          (GTK_CLIST (pTree), TRUE);
	//XX gtk_clist_set_compare_func       (GTK_CLIST (pTree), TreeCompare);
	//XX gtk_clist_set_column_auto_resize (GTK_CLIST (pTree), 0, TRUE);
	//XX gtk_clist_column_titles_passive  (GTK_CLIST (pTree));


	//XX CNode *pNode = new CNode (pTree);

	//XX GtkCTreeNode *pSibling = NULL;

	while (gtk_events_pending())
		gtk_main_iteration();
	
	{
		GdkCursor *cursor = gdk_cursor_new (GDK_WATCH);
		gdk_window_set_cursor (pWindow->window, cursor);
		gdk_cursor_destroy (cursor);
	}

	GString *pOldLocation = g_string_new ("");
	GString *pNewLocation = g_string_new ("");
	GString *pFormat      = g_string_new ("");
	GString *pPath        = g_string_new ("");

	//XX gtk_clist_freeze (GTK_CLIST (pTree));
	int iCount = 0;
	char buffer[256];
	while (!bAbort)
	{
		cin.getline (buffer, sizeof (buffer));
		if (cin.eof())
		{
			break;
		}

		//XX Status eStatus = ParsePathName (buffer, pPath);
		ParsePathName (buffer, pPath);

		//g_print ("status = %d\n", eStatus);
		//XX pNode->AddNode (NULL, pSibling, pPath->str, eStatus);
		iCount++;
		//if ((iCount % 20) == 0)	// change this to a timer?
		g_string_assign (pNewLocation, pPath->str);
		basename (pNewLocation);
		if (strcmp (pNewLocation->str, pOldLocation->str) != 0)
		{
			g_string_assign (pOldLocation, pNewLocation->str);
			g_string_sprintf (pFormat, "Processing: %s", pOldLocation->str);

			gtk_label_set_text (GTK_LABEL (label), pFormat->str);
			//gtk_clist_thaw (GTK_CLIST (pTree));
			while (gtk_events_pending())
				gtk_main_iteration();
			//gtk_clist_freeze (GTK_CLIST (pTree));
		}
	}

	//if (bAbort)
	//{
		//while (cin.getline (buffer, sizeof (buffer) != cin.eof()));
	//}
	//XX gtk_clist_thaw (GTK_CLIST (pTree));

	//gtk_widget_show (pWindow);
	//gtk_widget_show (pScrolled);
	//gtk_widget_show (pTree);

	GdkColor  Red;
	GdkColor  Yellow;
	GdkColor  Green;
	GtkStyle *pDirStyle  = gtk_style_new ();

	memset (&Red,  0, sizeof (Red));

	Red.red   = 65535;
	Red.green =     0;
	Red.blue  =     0;

	Yellow.red   = 65535;
	Yellow.green = 65535;
	Yellow.blue  =     0;

	Green.red   =     0;
	Green.green = 65535;
	Green.blue  =     0;

	for (int i = GTK_STATE_NORMAL; i < GTK_STATE_INSENSITIVE; i++)
	{
		pDirStyle->fg[i]   = Red;
		pDirStyle->bg[i]   = Green;
		pDirStyle->base[i] = Yellow;
	}

	gtk_widget_set_style (pTree, pDirStyle);

	{
		GdkCursor *cursor = gdk_cursor_new (GDK_ARROW);
		gdk_window_set_cursor (pWindow->window, cursor);
		gdk_cursor_destroy (cursor);
	}
#endif
	char      buffer[256];
	Status    status = eFileError;
	GString  *path   = g_string_new (NULL);
	//GString  *file   = g_string_new (NULL);
	//TreeNode *node   = NULL;
	GNode    *root   = g_node_new (NULL);

	while (TRUE)
	{
		cin.getline (buffer, sizeof (buffer));
		if (cin.eof())
		{
			break;
		}
		//g_print ("%s\n", buffer);
		status = ParsePathName (buffer, path);

		//g_print ("%s\t%s\n", path->str, file->str);

		//char *base;
		//GNode *match = tree_node_find_node ();
		//node = tree_node_new (status, file->str);
		//g_node_append_data (root, node);
		tree_node_add (root, path->str, status);

		/*
		switch (status)
		{
			case eFileSame:  g_print ("same:  %s\n", path->str); break;
			case eFileDiff:  g_print ("diff:  %s\n", path->str); break;
			case eFileLeft:  g_print ("left:  %s\n", path->str); break;
			case eFileRight: g_print ("right: %s\n", path->str); break;
			case eFileType:  g_print ("type:  %s\n", path->str); break;
			default:         g_print ("error: %s\n", path->str); break;
		}
		*/
		//tree_node_free (node);
	}

	g_string_free (path, TRUE);

	//gtk_main();
	//XX delete pNode;

	//g_mem_profile();
	return 0;
}

