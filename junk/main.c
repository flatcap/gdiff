#define ENABLE_NLS 1

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <gnome.h>
#include <locale.h>
#include "global.h"
#include "tree.h"
#include "node.h"

#define MATCHES 8
#define APPNAME   "TestApp"
#define WINNAME   "GDiff"
#define VERSION   "0.0.1"
#define LOCALEDIR "/usr/src/src"

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

char           *base_left = NULL;
char           *base_right = NULL;

static gboolean bAbort = FALSE;

void 
destroy (GtkWidget * pWidget, gpointer  pData)
{
	gtk_main_quit ();
}

void 
abort2 (GtkWidget * pWidget, gpointer  pData)
{
	bAbort = TRUE;
}

void 
font_ok (GtkWidget * pWidget, gpointer  pData)
{
	GdkFont *font = gtk_font_selection_dialog_get_font      (GTK_FONT_SELECTION_DIALOG (pData));
	gchar   *name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG (pData)); 

	g_print ("name = %s (%p)\n", name, font);

	gtk_widget_destroy (GTK_WIDGET (pData));
}

Status 
ParsePathName (char buffer[], GString * path)
{
	regmatch_t      matches[MATCHES];
	Status          result = eFileError;

	g_return_val_if_fail (buffer, eFileError);
	g_return_val_if_fail (path,   eFileError);

	if (regexec (&reg_same, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileSame;
	}
	else if (regexec (&reg_diff, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileDiff;
	}
	else if (regexec (&reg_only, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileLeft;						// This may be overridden

	}
	else if (regexec (&reg_type, buffer, MATCHES, matches, 0) == 0)
	{
		result = eFileType;
	}
	else
	{
		result = eFileError;
	}

	if (result == eFileError)
	{
		g_string_truncate (path, 0);
	}
	else
	{
		int             l = 6;//strlen (base_left);				// Should be global
		int             r = 6;//strlen (base_right);
		GString        *file = g_string_new (buffer + matches[2].rm_so);

		g_string_assign (path, buffer + matches[1].rm_so);
		g_string_truncate (path, matches[1].rm_eo - matches[1].rm_so);
		g_string_append (path, G_DIR_SEPARATOR_S);

		g_string_truncate (file, matches[2].rm_eo - matches[2].rm_so);
		g_string_append (path, file->str);
		g_string_free (file, TRUE);

		if (strncmp (base_left, path->str, l) == 0)
		{
			g_string_erase (path, 0, l);				// Same, diff, type, and left only

		}
		else if (strncmp (base_right, path->str, r) == 0)
		{
			g_string_erase (path, 0, r);
			result = eFileRight;					// Right-only

		}
		else
		{
			result = eFileError;
		}
	}

	return result;
}

void 
quick_message (char *left, char *right, int fd, GtkWidget * parent)
{
	//char           *message = "Hello world";
	//GtkWidget      *dialog;
	//GtkWidget      *label;
	GtkWidget      *fontdlg;

	/* Create the widgets */
	//dialog = gtk_dialog_new ();
	/*
	dialog = gtk_window_new (GTK_WINDOW_DIALOG);
	gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
	gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (parent));
	gtk_window_set_default_size (GTK_WINDOW (dialog), 500, 50);
	gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
	label = gtk_label_new (message);
	*/

	//fontdlg = gtk_button_new_with_label ("Abort");
	fontdlg = gtk_font_selection_dialog_new ("la la la");
	//gtk_window_set_modal         (GTK_WINDOW (fontdlg), TRUE);
	//gtk_window_set_transient_for (GTK_WINDOW (fontdlg), GTK_WINDOW (parent));

	/* Ensure that the dialog box is destroyed when the user clicks ok. */
	//gtk_signal_connect_object ((GtkObject *) (fontdlg), "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), (GtkObject *) dialog);
	//gtk_container_add (GTK_CONTAINER (dialog), fontdlg);

	gtk_font_selection_dialog_set_font_name    (GTK_FONT_SELECTION_DIALOG (fontdlg),
		"-sony-fixed-medium-r-normal-*-*-230-*-*-c-*-jisx0201.1976-0");
		//"-Sony-Fixed-Medium-R-Normal--24-170-100-100-C-120-JISX0201.1976-0");

	gtk_signal_connect_object (GTK_OBJECT (GTK_FONT_SELECTION_DIALOG (fontdlg)->cancel_button), "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT (fontdlg));
	gtk_signal_connect        (GTK_OBJECT (GTK_FONT_SELECTION_DIALOG (fontdlg)->ok_button),     "clicked", GTK_SIGNAL_FUNC (font_ok),            fontdlg);

	/* Add the label, and show everything we've added to the dialog. */
	//gtk_container_add (GTK_CONTAINER (dialog), label);
	//gtk_widget_show_all (dialog);
	//gtk_main ();
	gtk_widget_show (fontdlg);

	//GdkFont *font = gtk_font_selection_dialog_get_font      (GTK_FONT_SELECTION_DIALOG (fontdlg));
	//gchar   *name = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG (fontdlg)); 
}

int 
main (int argc, char *argv[], char *env[])
{
	GtkWidget *window = NULL;

	//g_print ("locale = %s\n", setlocale (LC_MESSAGES, ""));

	bindtextdomain (APPNAME, LOCALEDIR);
	textdomain (APPNAME);

	//g_print ("diff = %s\n", _("_Different"));

	gnome_init (APPNAME, VERSION, argc, argv);

	if (argc != 3)
	{
		g_print ("Usage: prog left right\n");
		return 1;
	}

	base_left = argv[1];
	base_right = argv[2];

	//g_print ("left = %s, right = %s\n", base_left, base_right);

	window = gtk_window_new (GTK_WINDOW_POPUP);
	//gtk_window_set_default_size (GTK_WINDOW (window), 500, 800);
	gtk_window_set_default_size (GTK_WINDOW (window), 1, 1);
	gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);

	gtk_widget_show (window);
	global_init (window);
	gtk_widget_hide (window);

	{
		TreeDialog *tree = tree_dialog_new (window);

		tree_dialog_parse (tree, base_left, base_right);
		tree_dialog_draw  (tree, eFileAll);
	}

#if 0
	GtkWidget      *box = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), box);
	gtk_widget_show (box);

	GtkWidget      *label = gtk_label_new ("Processing...");
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 10);
	gtk_widget_show (label);

	GtkWidget      *button = gtk_button_new_with_label ("Abort");
	gtk_box_pack_end (GTK_BOX (box), button, FALSE, FALSE, 10);
	//GtkAllocation a = { -1, -1, 20, 20 };
	//gtk_widget_size_allocate (button, &a);
	gtk_widget_show (button);
	//button->allocation.width  = 20;
	//button->allocation.height = 20;
	gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (abort2), NULL);

	//quick_message (base_left, base_right, 0, window);
	//gtk_main();

	//GtkWidget *pScrolled = gtk_scrolled_window_new (NULL, NULL);
	//gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (pScrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	//gtk_container_add (GTK_CONTAINER(window), pScrolled);

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

	while (gtk_events_pending ())
		gtk_main_iteration ();

	{
		GdkCursor      *cursor = gdk_cursor_new (GDK_WATCH);
		gdk_window_set_cursor (window->window, cursor);
		gdk_cursor_destroy (cursor);

		cursor = gdk_cursor_new (GDK_LEFT_PTR);
		gdk_window_set_cursor (button->window, cursor);
		gdk_cursor_destroy (cursor);
	}

	GString        *pOldLocation = g_string_new ("");
	GString        *pNewLocation = g_string_new ("");
	GString        *pFormat = g_string_new ("");
	GString        *pPath = g_string_new ("");

	//XX gtk_clist_freeze (GTK_CLIST (pTree));
	int             iCount = 0;
	Status          status = eFileError;
	//GString  *path   = g_string_new (NULL);
	//GString  *file   = g_string_new (NULL);
	//TreeNode *node   = NULL;
	GNode          *root = g_node_new (NULL);
	char            buffer[256];
	while (!bAbort)
	{
		cin.getline (buffer, sizeof (buffer));
		if (cin.eof ())
		{
			break;
		}

		status = ParsePathName (buffer, pPath);

		//g_print ("path = %s (%d)\n", pPath->str, status);
		tree_node_add (root, pPath->str, status);
		//XX pNode->AddNode (NULL, pSibling, pPath->str, eStatus);
		iCount++;
		//if ((iCount % 20) == 0)       // change this to a timer?
		g_string_assign (pNewLocation, pPath->str);
		char           *base = g_basename (pNewLocation->str);
		if (strcmp (base, pOldLocation->str) != 0)
		{
			g_string_assign (pOldLocation, base);
			g_string_sprintf (pFormat, "Processing: %s", pOldLocation->str);

			gtk_label_set_text (GTK_LABEL (label), pFormat->str);
			//gtk_clist_thaw (GTK_CLIST (pTree));
			while (gtk_events_pending ())
				gtk_main_iteration ();
			//gtk_clist_freeze (GTK_CLIST (pTree));
		}
	}

	if (bAbort)
	{
		tree_node_add (root, "Aborted!", eFileError);
	}

	tree_print (root, 0);
#endif

#if 0
	//if (bAbort)
	//{
	//while (cin.getline (buffer, sizeof (buffer) != cin.eof()));
	//}
	//XX gtk_clist_thaw (GTK_CLIST (pTree));

	//gtk_widget_show (window);
	//gtk_widget_show (pScrolled);
	//gtk_widget_show (pTree);

	GdkColor        Red;
	GdkColor        Yellow;
	GdkColor        Green;
	GtkStyle       *pDirStyle = gtk_style_new ();

	memset (&Red, 0, sizeof (Red));

	Red.red = 65535;
	Red.green = 0;
	Red.blue = 0;

	Yellow.red = 65535;
	Yellow.green = 65535;
	Yellow.blue = 0;

	Green.red = 0;
	Green.green = 65535;
	Green.blue = 0;

	for (int i = GTK_STATE_NORMAL; i < GTK_STATE_INSENSITIVE; i++)
	{
		pDirStyle->fg[i] = Red;
		pDirStyle->bg[i] = Green;
		pDirStyle->base[i] = Yellow;
	}

	gtk_widget_set_style (pTree, pDirStyle);

	{
		GdkCursor      *cursor = gdk_cursor_new (GDK_LEFT_PTR);
		gdk_window_set_cursor (window->window, cursor);
		gdk_cursor_destroy (cursor);

		cursor = gdk_cursor_new (GDK_LEFT_PTR);
		gdk_window_set_cursor (button->window, cursor);
		gdk_cursor_destroy (cursor);
	}

	while (TRUE)
	{
		cin.getline (buffer, sizeof (buffer));
		if (cin.eof ())
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

		/*
		 * switch (status)
		 * {
		 * case eFileSame:  g_print ("same:  %s\n", path->str); break;
		 * case eFileDiff:  g_print ("diff:  %s\n", path->str); break;
		 * case eFileLeft:  g_print ("left:  %s\n", path->str); break;
		 * case eFileRight: g_print ("right: %s\n", path->str); break;
		 * case eFileType:  g_print ("type:  %s\n", path->str); break;
		 * default:         g_print ("error: %s\n", path->str); break;
		 * }
		 */
		//tree_node_free (node);
	}

	g_string_free (path, TRUE);
#endif

	gtk_main();
	//XX delete pNode;

	//g_mem_profile();
	return 0;
}
