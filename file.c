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
	GtkWidget   *app    = NULL;
	GnomeApp    *g      = NULL;
	GtkMenuBar  *m      = NULL;
	GtkMenuShell*s      = NULL;
	GList       *l      = NULL;
	GtkMenuItem *i      = NULL;
	GtkWidget   *w      = NULL;
	char        *n      = NULL;
	GtkLabel    *label  = NULL;
	GtkBin      *bin    = NULL;
	GtkWidget   *child  = NULL;
	char        *text   = NULL;

	gnome_init (APPNAME, VERSION, argc, argv);

	app = gnome_app_new (APPNAME, WINNAME);

	gtk_signal_connect (GTK_OBJECT (app), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);

	gtk_window_set_default_size	(GTK_WINDOW (app), 200, 500);
	menu_create			(GNOME_APP  (app));

	g = GNOME_APP (app);
	m = GTK_MENU_BAR (g->menubar);
	s = GTK_MENU_SHELL (m);
	l = s->children;
	i = GTK_MENU_ITEM (l->data);
	w = GTK_WIDGET (i);
	n = w->name;
	bin = GTK_BIN (i);
	child = bin->child;
	label = GTK_LABEL (child);
	text = label->label;

	g_print ("menu       = %p\n", m);
	g_print ("menu shell = %p\n", s);
	g_print ("children   = %p\n", l);
	g_print ("item       = %p\n", i);
	g_print ("widget     = %p\n", w);
	g_print ("name       = %s\n", n);
	g_print ("bin        = %p\n", bin);
	g_print ("child      = %p\n", child);
	g_print ("label      = %p\n", label);
	g_print ("text       = %s\n", text);

	gtk_widget_show_all (app);
	gtk_main();

	return 0;
}

void main_diff (void)
{
	g_print ("main_diff\n");
}

