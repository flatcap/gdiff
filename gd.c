#include <gnome.h>
#include <locale.h>
#include "config.h"
#include "args.h"

#define WINNAME "Graphical differences"

void 
destroy (GtkObject * object, gpointer data)
{
	gtk_main_quit();
}

int 
main (int argc, char *argv[], char *envv[])
{
	GtkWidget *app = NULL;

	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

	gnome_init (PACKAGE, VERSION, argc, argv);

	app = gnome_app_new (PACKAGE, WINNAME);
	gtk_signal_connect (GTK_OBJECT (app), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
	gtk_widget_show_all (app);

	gtk_main();
	return 0;
}

