#include <gnome.h>
#include <regex.h>
#include "derived.h"
#include "spawn.h"
#include "progress.h"
#include "global.h"
#include "node.h"
#include "tree.h"
#include "menu.h"

static void gtk_diff_tree_init		 (GtkDiffTree * diff_tree);
static void gtk_diff_tree_class_init	 (GtkDiffTreeClass * klass);
static void gtk_diff_tree_finalize	 (GtkObject * object);

#define MATCHES 8

enum
{
	POPULATE,
	LAST_SIGNAL
};

static guint difftree_signals[LAST_SIGNAL] = { 0 };

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
	return gtk_diff_tree_new_with_titles (columns, tree_column, NULL, NULL);
}

GtkWidget *
gtk_diff_tree_new_with_titles (gint columns, gint tree_column, gchar *titles[], DiffOptions *diff)
{
	GtkWidget *widget = NULL;
	GtkCList  *list	  = NULL;
	GtkCTree  *tree	  = NULL;
	GtkDiffTree *difftree = NULL;

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
	difftree = GTK_DIFF_TREE (widget);

	list->columns     = columns;
	tree->tree_column = tree_column;
	difftree->diff = diff;

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
						sizeof (DiffTreeRow) * 512, 
						G_ALLOC_AND_FREE);
	//g_print ("chunk = %p\n", list->row_mem_chunk);
	//g_print ("chunk1 = %p\n", g_chunk_new (DiffTreeRow, list->row_mem_chunk));
	//g_print ("chunk2 = %p\n", g_chunk_new (DiffTreeRow, list->row_mem_chunk));
	//g_print ("chunk3 = %p\n", g_chunk_new (DiffTreeRow, list->row_mem_chunk));
	//g_print ("chunk4 = %p\n", g_chunk_new (GtkCTreeRow, list->row_mem_chunk));
	//g_print ("chunk5 = %p\n", g_chunk_new (GtkCTreeRow, list->row_mem_chunk));
	//g_print ("chunk6 = %p\n", g_chunk_new (GtkCListRow, list->row_mem_chunk));
	//g_print ("chunk7 = %p\n", g_chunk_new (GtkCListRow, list->row_mem_chunk));
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
	//g_print ("gtk_diff_tree_display\n");
	tree_dialog_draw (tree, eFileAll);

	//g_mem_chunk_print (GTK_CLIST (tree)->row_mem_chunk);
	//g_print ("sizeof (DiffTreeRow) = %d\n", sizeof (DiffTreeRow));
	//g_print ("sizeof (GtkCTreeRow) = %d\n", sizeof (GtkCTreeRow));
	//g_print ("sizeof (GtkCListRow) = %d\n", sizeof (GtkCListRow));
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
GtkStatusbar *
get_view_statusbar(GtkDiffTree *tree)
{
	//what if they change mode whilst we're working?
	GnomeMDIMode mode = GNOME_MDI_NOTEBOOK;		// derive from the tree's mdi child parent

	// all we can do, is climb that tree!
	GtkScrolledWindow *scroll   = NULL;
	GtkNotebook	  *notebook = NULL;
	GnomeDock	  *dock	    = NULL;
	GtkVBox		  *vbox	    = NULL;
	GnomeApp	  *app	    = NULL;
	GtkStatusbar	  *status   = NULL;

	g_return_val_if_fail (tree != NULL, NULL);

	g_print ("before horrendous code\n");

	scroll		= GTK_SCROLLED_WINDOW	(GTK_WIDGET (tree)	->parent);
	if (mode == GNOME_MDI_NOTEBOOK)
	{
		notebook= GTK_NOTEBOOK		(GTK_WIDGET (scroll)	->parent);
		dock	= GNOME_DOCK		(GTK_WIDGET (notebook)	->parent);
	}
	else
	{
		dock	= GNOME_DOCK		(GTK_WIDGET (scroll)	->parent);
	}
	vbox		= GTK_VBOX		(GTK_WIDGET (dock)	->parent);
	app		= GNOME_APP		(GTK_WIDGET (vbox)	->parent);

	status		= GTK_STATUSBAR		(app->statusbar);

	g_print ("after horrendous code\n");

	return status;
}

void
gtk_diff_tree_compare(GtkDiffTree *tree, char *left, char *right)
{
	g_return_if_fail (tree  != NULL);
	g_return_if_fail (left  != NULL);
	g_return_if_fail (right != NULL);

	g_free (tree->left);
	g_free (tree->right);

	g_print ("before signal emit\n");
	gtk_signal_emit (GTK_OBJECT (tree), difftree_signals[POPULATE], 0x1234);
	g_print ("after signal emit\n");
	//tree->left  = dup_and_add_slash (left);
	//tree->right = dup_and_add_slash (right);
	tree->left  = g_strdup (left);
	tree->right = g_strdup (right);

	{
	char	 buffer[_POSIX_PATH_MAX * 2 + 50]; // XXX possible buffer overrun here
	FILE	*f       = NULL;
	char    *base    = NULL;
	GString *path    = g_string_new (NULL);
	GString *old_loc = g_string_new (NULL);
	GString *new_loc = g_string_new (NULL);
	Status   status  = eFileError;
	Progress *progress = NULL;

	progress = progress_new (get_view_statusbar(tree));
	//progress = progress_new (global_statusbar);

	// P -> no 'right' files! will need to stat left/right files
	//f = run_diff (g_strdup_printf ("diff -qrsP %s %s", tree->left, tree->right));
	f = run_diff (g_strdup_printf ("diff -qrs %s %s", tree->left, tree->right));
	//f = stdin;

	g_return_if_fail (f);

	tree->root = g_node_new (NULL);
	g_return_if_fail (tree->root);

	while (fgets (buffer, sizeof (buffer), f))
	{
		status = gtk_diff_tree_parse_line (tree, buffer, path);
		//g_print ("parsed: %s\n", path->str);

		tree_node_add (tree->root, path->str, status, path->str);

		g_string_assign (new_loc, path->str);

		base = g_basename (new_loc->str);
		if (base == new_loc->str)			// just a file in the root directory
		{
			g_string_assign (new_loc, ".");
		}

		g_string_truncate (new_loc, (base - new_loc->str) - 1);
		//g_print ("new_loc = %s, old_loc = %s\n", new_loc->str, old_loc->str);

		if ((strcmp (new_loc->str, old_loc->str) != 0))// &&
		    //(strlen (new_loc->str) > strlen (old_loc->str)))		// files between dirs in list!
		{
			//g_print ("progress %s\n", new_loc->str);
			progress_set_text (progress, new_loc->str);
			g_string_assign (old_loc, new_loc->str);
		}
	}

	fclose (f);			// temp
	//tree_print (tree->root, 0);	// temp

	progress_free (progress);
	gtk_diff_tree_display (tree);
	}
	g_print ("leaving method (after signal emit)\n");
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
	tree->diff  = NULL;

	//g_print ("gtk_diff_tree_init\n");
}

static gint (* old_button_handler)  (GtkWidget *widget, GdkEventButton *event) = NULL;
static gint (* old_key_handler)     (GtkWidget *widget, GdkEventKey    *event) = NULL;
static void (* old_show_handler)    (GtkWidget *widget)                        = NULL;
static void (* old_realize_handler) (GtkWidget *widget)                        = NULL;
static void (* old_draw_handler)    (GtkWidget *widget, GdkRectangle *area)    = NULL;


#if 0
double click prototype needs to be:
int handler (GtkWidget *tree, TreeNode *node);
#endif

void
gtk_diff_tree_show (GtkWidget *widget)
{
	g_print ("gtk_diff_tree_show\n");
	old_show_handler (widget);
}

void
gtk_diff_tree_draw (GtkWidget *widget, GdkRectangle *area)
{
	static gboolean drawn = FALSE;
	GtkDiffTree *tree = GTK_DIFF_TREE (widget);

	g_print ("gtk_diff_tree_draw\n");

	if (!drawn)
	{
		gtk_diff_tree_compare (tree, tree->diff->left, tree->diff->right);
		drawn = TRUE;
	}

	old_draw_handler (widget, area);
}

void
gtk_diff_tree_realize (GtkWidget *widget)
{
	g_print ("gtk_diff_tree_realize\n");
	old_realize_handler (widget);
}


gint
gtk_diff_tree_key_press_event (GtkWidget *widget, GdkEventKey *event)
{
	//g_print ("key press\n");
	return old_key_handler (widget, event);
}

gint
gtk_diff_tree_button_press_event (GtkWidget *widget, GdkEventButton *event)
{
	gboolean result = FALSE;

	//g_print ("button %d\n", event->button);	// 1,2,3
	//g_print ("state %d\n", event->state);	// shift, ctrl, alt etc
	if (event->type == GDK_2BUTTON_PRESS)
	{
		// my handler
		g_print ("double click\n");
		result = TRUE;
	}
	/*
	else if (event->type == GDK_3BUTTON_PRESS)
	{
		g_print ("triple click\n");
		result = TRUE;
	}
	else if (event->type == GDK_BUTTON_PRESS)
	{
		g_print ("single click\n");
		result 	= TRUE;
	}
	*/
	else
	{
		result = old_button_handler (widget, event);
	}

	return result;
}

static void
gtk_diff_tree_class_init (GtkDiffTreeClass * klass)
{
	GtkObjectClass *object_class = NULL;
	GtkWidgetClass *widget_class = NULL;

	g_return_if_fail (klass != NULL);

	//g_print ("gtk_diff_tree_class_init\n");

	// override methods
	object_class = (GtkObjectClass*) klass;
	object_class->finalize = gtk_diff_tree_finalize;

	widget_class = (GtkWidgetClass*) klass;

	old_button_handler = widget_class->button_press_event;
	old_key_handler    = widget_class->key_press_event;
	old_show_handler   = widget_class->show;
	old_realize_handler= widget_class->realize;
	old_draw_handler   = widget_class->draw;

	widget_class->button_press_event = gtk_diff_tree_button_press_event;
	widget_class->key_press_event    = gtk_diff_tree_key_press_event;
	widget_class->show               = gtk_diff_tree_show;
	widget_class->draw               = gtk_diff_tree_draw;
	widget_class->realize            = gtk_diff_tree_realize;

	klass->populate_handler = gtk_diff_tree_real_populate;

	difftree_signals [POPULATE] = gtk_signal_new ("populate",
						      GTK_RUN_FIRST,
						      object_class->type,
						      GTK_SIGNAL_OFFSET (GtkDiffTreeClass, populate_handler),
						      gtk_marshal_NONE__POINTER,
						      GTK_TYPE_NONE, 1,
						      GTK_TYPE_POINTER);
	g_print ("class init: signal = %d\n", difftree_signals[POPULATE]);
	gtk_object_class_add_signals (object_class, difftree_signals, LAST_SIGNAL);
}

void
gtk_diff_tree_real_populate (GtkWidget *widget, gpointer data)
{
	g_print ("POPULATE!!! (%p)\n", data);
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

