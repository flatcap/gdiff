#include "progress.h"

GtkWidget *
progress_new (void)
{
	return NULL;
}

void
progress_set_text (GtkWidget *progress, char *text)
{
	while (gtk_events_pending())
	{
		gtk_main_iteration();
	}
}

