#include <gnome.h>
#include "progress.h"

GtkWidget *
progress_new (void)
{
	return NULL;
}

void
progress_set_text (GtkWidget *progress, char *text)
{
	//status bar or modeless dialog?
	//g_print ("%s\n", text);
	while (gtk_events_pending())
	{
		gtk_main_iteration();
	}
}

