#include <gnome.h>
#include "progress.h"

Progress *
progress_new (GtkStatusbar *status)
{
	Progress *progress = NULL;

	progress = g_malloc (sizeof (Progress));

	progress->status  = status;
	progress->message = NULL;
	progress->context = 0;

	return progress;
}

void
progress_free (Progress *progress)
{
	gtk_statusbar_pop (progress->status, progress->context);
	while (gtk_events_pending())
	{
		gtk_main_iteration();
	}

	g_free (progress->message);
	g_free (progress);
}

void
progress_set_text (Progress *progress, char *text)
{
	guint msgid = 0;
	char *message = NULL;
	
	if (progress->context)
	{
		gtk_statusbar_pop (progress->status, progress->context);
	}
	else
	{
		progress->context = gtk_statusbar_get_context_id (progress->status, "Some context?");
	}

	message = g_strdup_printf ("Comparing: %s", text);
	msgid = gtk_statusbar_push (progress->status, progress->context, message);

	//g_print ("%s\n", message);
	while (gtk_events_pending())
	{
		gtk_main_iteration();
	}
	g_free (message);
}

