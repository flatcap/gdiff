#include <gnome.h>

int main (int argc, char *argv[])
{
	GtkWidget *pApp = NULL;
	GtkWidget *pColor = NULL;

	gnome_init ("Test", "1.23", argc, argv);

	pApp = gnome_app_new ("Test", "Title");

	gtk_widget_show (pApp);

	pColor = gnome_color_picker_new();
	gtk_widget_show (pColor);

	gtk_main();
	return 0;
}
