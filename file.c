#include <gnome.h>
#include "menu.h"

#define APPNAME "derived"
#define VERSION "0.0.1"
#define WINNAME "Derived"

void
destroy (GtkWidget * pWidget, gpointer * pData)
{
	gtk_main_quit();
}

int
main (int argc, char *argv[])
{
	GtkWidget *app    = NULL;
	//GtkWidget *status = NULL;

	gnome_init (APPNAME, VERSION, argc, argv);

	app = gnome_app_new (APPNAME, WINNAME);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);

	gtk_window_set_default_size	(GTK_WINDOW (app), 200, 500);
	menu_create			(GNOME_APP  (app));

	gtk_widget_show_all (app);
	gtk_main();

	return 0;
}

void main_diff (void)
{
	g_print ("main_diff\n");
}

