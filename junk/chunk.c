#include <gnome.h>

#define APPNAME "test"
#define VERSION "0.0.1"

int main (int argc, char *argv[])
{
	GtkWidget *w = NULL;
	GtkCTree  *t = NULL;
	GtkCList  *l = NULL;

	GMemChunk mr = NULL;
	GMemChunk mc = NULL;

	gnome_init (APPNAME, VERSION, argc, argv);

	w = gtk_ctree_new (2, 1);
	t = GTK_CTREE (w);
	l = GTK_CLIST (t);

	g_print ("%p\n", l->row_mem_chunk);
	g_print ("%p\n", l->cell_mem_chunk);

	mr = g_mem_chunk_new ("Richard test row",
			      sizeof (GtkCTreeRow),
			      sizeof (GtkCTreeRow) * 128,
			      G_ALLOC_AND_FREE);

	mc = g_mem_chunk_new ("Richard test cell",
			      sizeof (GtkCell),
			      sizeof (GtkCell) * 128,
			      G_ALLOC_AND_FREE);

	g_mem_chunk_destroy (mr);
	g_mem_chunk_destroy (mc);

	return 0;
}

