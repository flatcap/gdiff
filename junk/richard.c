#include <gnome.h>
#include "derived.h"
#include "menu.h"

#define APPNAME "derived"
#define VERSION "0.0.1"
#define WINNAME "Derived"

void
destroy (GtkWidget * pWidget, gpointer  pData)
{
	gtk_main_quit ();
}

int
main (int argc, char *argv[])
{
	static char *cols[] = { "Left", "Right" };
	GtkWidget   *app    = NULL;
	GtkWidget   *tree   = NULL;

	gnome_init (APPNAME, VERSION, argc, argv);

	app = gnome_app_new (APPNAME, WINNAME);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);

	tree = gtk_diff_tree_new_with_titles (2, 0, cols);

	gtk_window_set_default_size	(GTK_WINDOW (app), 200, 500);
	gnome_app_set_contents		(GNOME_APP  (app), tree);
	menu_create			(GNOME_APP  (app));

	gtk_widget_show_all (app);
	gtk_main();

	return 0;
}

