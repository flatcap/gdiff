#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gnome.h>
#include "tree.h"
#include "node.h"
#include "global.h"

#define MATCHES 8
#define APPNAME "TestApp"
#define WINNAME "GDiff"
#define VERSION "0.0.1"

// Local methods
Status tree_dialog_parse_diff_line (TreeDialog *tree, char *buffer, GString *path);

void file_open_cb  (GtkWidget * widget, gpointer data);
void file_close_cb (GtkWidget * widget, gpointer data);
void about_cb      (GtkWidget * widget, gpointer data);
void color_cb      (GtkWidget * widget, gpointer data);
void properties_cb (GtkWidget * widget, gpointer data);
void options_cb    (GtkWidget * widget, gpointer data);
void view_cb       (GtkWidget * widget, gpointer data);

GnomeUIInfo     file_menu[] =
{
	GNOMEUIINFO_MENU_OPEN_ITEM (file_open_cb, NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM (file_close_cb, NULL),
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_MENU_PROPERTIES_ITEM (properties_cb, NULL),
	GNOMEUIINFO_SEPARATOR,
	GNOMEUIINFO_ITEM ("_Change Color", NULL, color_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo     help_menu[] =
{
	GNOMEUIINFO_MENU_ABOUT_ITEM (about_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo view_menu[] = {
	GNOMEUIINFO_TOGGLEITEM("_Toolbar", "tooltip1", view_cb, NULL),//NULL=pixmap
	GNOMEUIINFO_TOGGLEITEM("_Tooltips","tooltip4", view_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo options_menu[] = {
	GNOMEUIINFO_TOGGLEITEM("_Different", "tooltip1", options_cb, NULL),//NULL=pixmap
	GNOMEUIINFO_TOGGLEITEM("_Same",      "tooltip2", options_cb, NULL),
	GNOMEUIINFO_TOGGLEITEM("_Left only", "tooltip3", options_cb, NULL),
	GNOMEUIINFO_TOGGLEITEM("_Right only","tooltip4", options_cb, NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo     main_menu[] =
{
	GNOMEUIINFO_MENU_FILE_TREE (file_menu),
	GNOMEUIINFO_SUBTREE("_Options", options_menu),
	GNOMEUIINFO_SUBTREE("_View", view_menu),
	GNOMEUIINFO_MENU_HELP_TREE (help_menu),
	GNOMEUIINFO_END
};

void file_open_cb (GtkWidget * widget, gpointer data)
{
	g_print ("file_open_cb (%p)\n", data);
}

void file_close_cb (GtkWidget * widget, gpointer data)
{
	g_print ("file_close_cb (%p)\n", data);
}

void about_cb (GtkWidget * widget, gpointer data)
{
	g_print ("about_cb (%p)\n", data);
}

void color_cb (GtkWidget * widget, gpointer data)
{
	g_print ("color_cb (%p)\n", data);
}

void view_cb (GtkWidget * widget, gpointer data)
{
	g_print ("view_cb (%p)\n", data);
}

void options_cb (GtkWidget * widget, gpointer data)
{
	GtkCheckMenuItem *button = GTK_CHECK_MENU_ITEM (widget);
	//GtkButton      *button = (GtkButton *) data;
	//GtkWidget      *child = GTK_BIN (button)->child;
	//GtkLabel       *label = GTK_LABEL (child);

	char *name = gtk_widget_get_name (widget);
	g_print ("options_cb (%p)\n", data);

	//g_print ("options_cb name '%s', button %p\n", name, button);
	if (button)
	{
		gboolean active = button->active;
		g_print ("options_cb name '%s', button %p, active %d\n", name, button, active);

		//gtk_check_menu_item_set_show_toggle (button, TRUE);
		//gtk_check_menu_item_set_active (button, !active);
	}

	//g_print ("options_cb press '%s'\n", label->label);
}

void properties_cb (GtkWidget * widget, gpointer data)
{
	g_print ("properties_cb (%p)\n", data);
}

gint 
TreeCompare (GtkCList * clist, gconstpointer ptr1, gconstpointer ptr2)
{
	GtkCTreeRow    *row1 = (GtkCTreeRow *) ptr1;
	GtkCTreeRow    *row2 = (GtkCTreeRow *) ptr2;

	int             leaf1 = (row1->is_leaf);
	int             leaf2 = (row2->is_leaf);

	char           *text1 = GTK_CELL_PIXTEXT (row1->row.cell[clist->sort_column])->text;
	char           *text2 = GTK_CELL_PIXTEXT (row2->row.cell[clist->sort_column])->text;

	if (leaf1 != leaf2)							// One leaf and one node

	{
		return ((leaf1) ? 1 : -1);
	}
	else
	{
		return strcmp (text1, text2);
	}
}

TreeDialog     *
tree_dialog_new (GtkWidget *parent)
{
	TreeDialog *tree   = (TreeDialog *) g_malloc (sizeof (TreeDialog));
	GtkWidget  *dialog = NULL;

	g_return_val_if_fail (tree, NULL);

	tree->parent	= parent;
	tree->dialog	= NULL;
	tree->ctree	= NULL;
	tree->left	= NULL;
	tree->right	= NULL;
	tree->root	= NULL;

	//dialog = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	dialog = gnome_app_new (APPNAME, WINNAME);
	gnome_app_create_menus_with_data (GNOME_APP (dialog), main_menu, tree);
	//gnome_app_install_menu_hints     (GNOME_APP (dialog), main_menu);

	g_print ("tree = %p\n", tree);
	if (dialog)
	{
		char *cols[] = {"Name", "Description"};
		GtkWidget *scrolled = NULL;
		GtkWidget *ctree    = NULL;

		gtk_window_set_default_size (GTK_WINDOW (dialog), 300, 600);

		//gtk_signal_connect (GTK_OBJECT (dialog), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);

		scrolled = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		//gtk_container_add (GTK_CONTAINER(dialog), scrolled);
		gnome_app_set_contents (GNOME_APP (dialog), scrolled);
		gtk_widget_show (scrolled);

		ctree = gtk_ctree_new_with_titles (2, 0, cols);
		//gtk_ctree_set_line_style (GTK_CTREE (ctree), GTK_CTREE_LINES_TABBED);
		gtk_clist_set_selection_mode     (GTK_CLIST (ctree), GTK_SELECTION_BROWSE);
		gtk_clist_set_auto_sort          (GTK_CLIST (ctree), TRUE);
		gtk_clist_set_compare_func       (GTK_CLIST (ctree), TreeCompare);
		gtk_clist_set_column_auto_resize (GTK_CLIST (ctree), 0, TRUE);
		gtk_clist_column_titles_passive  (GTK_CLIST (ctree));

		gtk_container_add (GTK_CONTAINER (scrolled), ctree);
		gtk_widget_show (ctree);

		tree->dialog = dialog;
		tree->ctree  = ctree;
	}
	else
	{
		g_free (tree);
		tree = NULL;
	}

	return tree;
}

FILE *
tree_dialog_start_diff (TreeDialog *tree, char *options)
{
	return stdin;
}

//temp
void tree_print (GNode *node, int depth);

gboolean 
tree_dialog_parse (TreeDialog *tree, char *left, char *right)
{
	char	 buffer[_POSIX_PATH_MAX * 2 + 50];
	FILE	*f       = NULL;
	char    *base    = NULL;
	GString *path    = g_string_new (NULL);
	GString *format  = g_string_new (NULL);
	GString *old_loc = g_string_new (NULL);
	GString *new_loc = g_string_new (NULL);
	Status   status  = eFileError;

	g_free (tree->left);
	g_free (tree->right);

	tree->left  = g_strdup (left);
	tree->right = g_strdup (right);

	f = tree_dialog_start_diff (tree, "-qrs");
	g_return_val_if_fail (f, FALSE);

	tree->root = g_node_new (NULL);
	g_return_val_if_fail (tree->root, FALSE);

	while (fgets (buffer, sizeof (buffer), f))
	{
		status = tree_dialog_parse_diff_line (tree, buffer, path);

		tree_node_add (tree->root, path->str, status);

		g_string_assign (new_loc, path->str);

		base = g_basename (new_loc->str);

		if (strcmp (base, old_loc->str) != 0)
		{
			g_string_assign (old_loc, base);
			g_string_sprintf (format, "Processing: %s", old_loc->str);

			//gtk_label_set_text (GTK_LABEL (label), format->str);
			//while (gtk_events_pending ())
				//gtk_main_iteration ();
			//g_print ("%s\n", format->str);
		}
	}

	//fclose (f);			// temp
	//tree_print (tree->root, 0);	// temp

	return TRUE;
}

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
tree_dialog_traverse (GtkCTree *ctree, GtkCTreeNode *parent, GNode *node, Status status)
{
	//GtkCTreeNode *new_node = NULL;
	GtkCTreeNode *sibling  = NULL;
	TreeNode     *tnode    = NULL;
	char         *text[2]  = { NULL, NULL };

	g_return_if_fail (ctree);
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
					sibling = gtk_ctree_insert_node (ctree, parent, sibling, text, 5, pixmap_closed, mask_closed, pixmap_open, mask_open, FALSE, FALSE);
				}
			}
			else
			{
				if (status & tnode->status)
				{
					text[1] = get_status_text (tnode->status, status, FALSE);
					sibling = gtk_ctree_insert_node (ctree, parent, sibling, text, 5, pixmap_leaf, mask_leaf, NULL, NULL, TRUE, FALSE);
					g_free (text[1]);
					//g_print ("%s (%d) %p\n", tnode->name, tnode->status, sibling);
				}
			}

			if ((sibling) && (status & tnode->status))
			//if (sibling)
			{ 
				GtkStyle *style = get_status_style (tnode->status, (node->children != NULL));
				gtk_ctree_node_set_cell_style (ctree, sibling, 0, style);
				gtk_style_unref (style);
				//g_print ("get_status_style (%d, %d);\n", tnode->status, (node->children != NULL));
			}
		}

		if (node->children)
		{
			tree_dialog_traverse (ctree, sibling, node->children, status);
		}

		node = node->next;
	}
}

gboolean 
tree_dialog_draw (TreeDialog *tree, Status status)
{
	g_return_val_if_fail (tree,         FALSE);
	g_return_val_if_fail (tree->dialog, FALSE);
	g_return_val_if_fail (tree->ctree,  FALSE);

	tree->view = status;

	//g_print ("max height = %d\n", g_node_max_height (tree->root));
	//g_print ("root = %p\n", tree->root);
	//g_print ("children = %p\n", tree->root->children);
	//g_print ("prev = %p\n", tree->root->prev);
	//g_print ("next = %p\n", tree->root->next);

	gtk_widget_show (tree->dialog);
	gtk_widget_show (tree->ctree);

	//tree_print (tree->root, 0);

	gtk_clist_freeze     (GTK_CLIST (tree->ctree));
	gtk_clist_clear      (GTK_CLIST (tree->ctree));
	tree_dialog_traverse (GTK_CTREE (tree->ctree), NULL, tree->root, status);
	gtk_clist_thaw       (GTK_CLIST (tree->ctree));

	return TRUE;
}

void 
tree_dialog_free (TreeDialog *tree)
{
	g_free (tree->left);
	g_free (tree->right);

	//node_free (tree->root);

	g_free (tree);
}

Status
tree_dialog_parse_diff_line (TreeDialog *tree, char *buffer, GString *path)
{
	regmatch_t      matches[MATCHES];
	Status          result = eFileError;

	g_return_val_if_fail (buffer, eFileError);
	g_return_val_if_fail (path, eFileError);

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
		int l = strlen (tree->left);
		int r = strlen (tree->right);

		GString        *file = g_string_new (buffer + matches[2].rm_so);

		g_string_assign (path, buffer + matches[1].rm_so);
		g_string_truncate (path, matches[1].rm_eo - matches[1].rm_so);
		g_string_append (path, G_DIR_SEPARATOR_S);

		g_string_truncate (file, matches[2].rm_eo - matches[2].rm_so);
		g_string_append (path, file->str);
		g_string_free (file, TRUE);

		if (strncmp (tree->left, path->str, l) == 0)
		{
			g_string_erase (path, 0, l);				// Same, diff, type, and left only

		}
		else if (strncmp (tree->right, path->str, r) == 0)
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

