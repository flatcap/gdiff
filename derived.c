#include <gnome.h>
#include <regex.h>
#include "derived.h"
#include "spawn.h"
#include "progress.h"
#include "global.h"

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
						sizeof (GtkCListRow),
						sizeof (GtkCListRow) * 128, 
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

void
gtk_diff_tree_compare(GtkDiffTree *tree, char *left, char *right)
{
	g_return_if_fail (tree  != NULL);
	g_return_if_fail (left  != NULL);
	g_return_if_fail (right != NULL);

	g_free (tree->left);
	g_free (tree->right);

	tree->left  = g_strdup (left);
	tree->right = g_strdup (right);

	// GtkWidget *progress = progress_new ();
	{
	char	 buffer[_POSIX_PATH_MAX * 2 + 50];
	FILE	*f       = NULL;
	char    *base    = NULL;
	GString *path    = g_string_new (NULL);
	GString *format  = g_string_new (NULL);
	GString *old_loc = g_string_new (NULL);
	GString *new_loc = g_string_new (NULL);
	Status   status  = eFileError;

	f = spawn_diff ("-qrs", tree->left, tree->right);

	g_return_if_fail (f);

	//tree->root = g_node_new (NULL);
	//g_return_if_fail (tree->root);

	while (fgets (buffer, sizeof (buffer), f))
	{
		status = gtk_diff_tree_parse_line (tree, buffer, path);

		//tree_node_add (tree->root, path->str, status);

		g_string_assign (new_loc, path->str);

		base = g_basename (new_loc->str);

		if (strcmp (base, old_loc->str) != 0)
		{
			g_string_assign (old_loc, base);
			g_string_sprintf (format, "Processing: %s", old_loc->str);

			//progress_set_text (progress, format->str);
			g_print ("%s\n", format->str);
		}
	}

	//fclose (f);			// temp
	//tree_print (tree->root, 0);	// temp
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

