#include <gnome.h>
#include <regex.h>
#include "derived.h"
#include "spawn.h"
#include "progress.h"
#include "global.h"
#include "node.h"
#include "tree.h"

static void gtk_diff_tree_init		 (GtkDiffTree * diff_tree);
static void gtk_diff_tree_class_init	 (GtkDiffTreeClass * class);
static void gtk_diff_tree_finalize	 (GtkObject * object);

#define MATCHES 8

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
gtk_diff_tree_new (gint columns, gint tree_column)
{
	//g_print ("gtk_diff_tree_new\n");
	return gtk_diff_tree_new_with_titles (columns, tree_column, NULL);
}

GtkWidget *
gtk_diff_tree_new_with_titles (gint columns, gint tree_column, gchar *titles[])
{
	GtkWidget *widget = NULL;
	GtkCList  *list	  = NULL;
	GtkCTree  *tree	  = NULL;

	g_return_val_if_fail (columns     >  0,       NULL);
	g_return_val_if_fail (tree_column >= 0,       NULL);
	g_return_val_if_fail (tree_column <  columns, NULL);

	//g_print ("gtk_diff_tree_new\n");

	widget = gtk_widget_new (GTK_TYPE_DIFF_TREE,
				 "n_columns",   columns,
				 "tree_column", tree_column,
				 NULL);

	g_return_val_if_fail (widget != NULL, NULL);

	list = GTK_CLIST (widget);
	tree = GTK_CTREE (widget);

	list->columns     = columns;
	tree->tree_column = tree_column;

	//gtk_ctree_construct (tree, columns, tree_column, titles);

	if (titles)
	{
		int i;
		for (i = 0; i < columns; i++)
		{
			//g_print ("col %d, title %s\n", i, titles[i]);
			gtk_clist_set_column_title (list, i, titles[i]);
		}

		gtk_clist_column_titles_show (list);

		//g_print ("row = %p, cell = %p\n", list->row_mem_chunk, list->cell_mem_chunk);
	}

	// Nobody can be using these chunks, yet...
	g_mem_chunk_destroy (list->row_mem_chunk);
	list->row_mem_chunk = g_mem_chunk_new ("clist row mem chunk",
						sizeof (DiffTreeRow),
						sizeof (DiffTreeRow) * 128, 
						G_ALLOC_AND_FREE);

	return widget;
}

void
gtk_diff_tree_set_view (GtkDiffTree *tree, Status status)
{
	g_return_if_fail (tree != NULL);
	g_return_if_fail (status >= eFileSame);
	g_return_if_fail (status <= eFileAll);

	tree->view = status;
}

Status
gtk_diff_tree_parse_line (GtkDiffTree *tree, char *buffer, GString *path)
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
		result = eFileLeft;				// This may be overridden
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

		GString *file = g_string_new (buffer + matches[2].rm_so);

		//g_print ("	path = %s", buffer);
		g_string_assign (path, buffer + matches[1].rm_so);
		//g_print ("	path = %s", path->str);
		g_string_truncate (path, matches[1].rm_eo - matches[1].rm_so);
		//g_print ("	path = %s\n", path->str);
		g_string_append (path, G_DIR_SEPARATOR_S);
		//g_print ("	path = %s\n", path->str);

		g_string_truncate (file, matches[2].rm_eo - matches[2].rm_so);
		//g_print ("	file = %s\n", file->str);
		g_string_append (path, file->str);
		g_string_free (file, TRUE);

		if (strncmp (tree->left, path->str, l) == 0)
		{
			g_string_erase (path, 0, l + 1);			// Same, diff, type, and left only
		}								// include slash /
		else if (strncmp (tree->right, path->str, r) == 0)
		{
			g_string_erase (path, 0, r + 1);			// inlclude slash /
			result = eFileRight;					// Right-only
		}
		else
		{
			result = eFileError;
		}
	}

	//g_print ("path = %s\n", path->str);
	return result;
}

void
gtk_diff_tree_display (GtkDiffTree *tree)
{
	g_print ("gtk_diff_tree_display\n");
	tree_dialog_draw (tree, eFileAll);
}

char *
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

// TODO move this code out of the tree
// get someone else to do the spawn / parse stuff (MDI maybe?)
// gtk_diff_tree_display (TreeNode *nodes)
void
gtk_diff_tree_compare(GtkDiffTree *tree, char *left, char *right)
{
	g_return_if_fail (tree  != NULL);
	g_return_if_fail (left  != NULL);
	g_return_if_fail (right != NULL);

	g_free (tree->left);
	g_free (tree->right);

	//tree->left  = dup_and_add_slash (left);
	//tree->right = dup_and_add_slash (right);
	tree->left  = g_strdup (left);
	tree->right = g_strdup (right);

	{
	char	 buffer[_POSIX_PATH_MAX * 2 + 50];
	FILE	*f       = NULL;
	char    *base    = NULL;
	GString *path    = g_string_new (NULL);
	GString *old_loc = g_string_new (NULL);
	GString *new_loc = g_string_new (NULL);
	Status   status  = eFileError;
	GtkWidget *progress = NULL;

	progress = progress_new ();

	f = run_diff (g_strdup_printf ("diff -qrsP %s %s", tree->left, tree->right));
	//f = stdin;

	g_return_if_fail (f);

	tree->root = g_node_new (NULL);
	g_return_if_fail (tree->root);

	while (fgets (buffer, sizeof (buffer), f))
	{
		status = gtk_diff_tree_parse_line (tree, buffer, path);
		//g_print ("parsed: %s\n", path->str);

		tree_node_add (tree->root, path->str, status);

		g_string_assign (new_loc, path->str);

		base = g_basename (new_loc->str);

		g_string_truncate (new_loc, (base - new_loc->str) - 1);
		//g_print ("new_loc = %s, old_loc = %s\n", new_loc->str, old_loc->str);

		if ((strcmp (new_loc->str, old_loc->str) != 0))// &&
		    //(strlen (new_loc->str) > strlen (old_loc->str)))		// files between dirs in list!
		{
			progress_set_text (progress, new_loc->str);
			g_string_assign (old_loc, new_loc->str);
		}
	}

	fclose (f);			// temp
	//tree_print (tree->root, 0);	// temp

	gtk_diff_tree_display (tree);
	}
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

	//g_print ("gtk_diff_tree_init\n");
}

static void
gtk_diff_tree_class_init (GtkDiffTreeClass * class)
{
	GtkObjectClass *object_class = NULL;

	g_return_if_fail (class != NULL);

	// override methods
	object_class = (GtkObjectClass*) class;
	object_class->finalize = gtk_diff_tree_finalize;

	//g_print ("gtk_diff_tree_class_init\n");
}

static void
gtk_diff_tree_finalize (GtkObject *object)
{
	GtkObjectClass	*parent = NULL;
	//GtkDiffTree	*diff_tree = GTK_DIFF_TREE (object);

	parent = gtk_type_class (gtk_ctree_get_type());		// my parent class

	g_return_if_fail (parent != NULL);

	parent->finalize (object);
}

