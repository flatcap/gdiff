/*
    progress.c -- description
    Copyright (C) 1999  Richard Russon <richard.russon@ait.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Revision: 1.7 $ */

#include <gnome.h>
#include "progress.h"

/*----------------------------------------------------------------------------*/
Progress * progress_new (GtkStatusbar *status);
void progress_free (Progress *progress);
void progress_set_text (Progress *progress, char *text);
/*----------------------------------------------------------------------------*/

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

