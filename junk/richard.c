#include <gnome.h>
#include "derived.h"

#define APPNAME "derived"
#define VERSION "0.0.1"
#define WINNAME "Derived"

void 
destroy (GtkWidget * pWidget, gpointer * pData)
{
	gtk_main_quit ();
}

int main (int argc, char *argv[])
{
	//GtkWidget *app = NULL;

	gnome_init (APPNAME, VERSION, argc, argv);

#if 0
	app = gnome_app_new (APPNAME, WINNAME);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);

	gtk_widget_show (app);
	gtk_main();
#endif

	{
		GtkWidget *r = gtk_richard_new();

		gtk_object_ref   (GTK_OBJECT (r));
		gtk_object_unref (GTK_OBJECT (r));
		gtk_object_unref (GTK_OBJECT (r));
	}
	
	return 0;
}

