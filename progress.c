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

	g_return_if_fail (progress != NULL);

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

/*
	while (fgets (buffer, sizeof (buffer), file))
	{
		status = gtk_diff_tree_parse_line (left, right, buffer, path);
		//g_print ("parsed: %d, %s\n", status, path->str);

		parts = g_strsplit (path->str, G_DIR_SEPARATOR_S, -1);

		//new node;
		//node->left, right, relative, left_root?, right_root?, status

		//RR tree_node_add (node, path->str, status, path->str);

		g_string_assign (new_loc, path->str);

		base = g_basename (new_loc->str);
		if (base == new_loc->str)			// just a file in the root directory
		{
			g_string_assign (new_loc, ".");
		}

		g_string_truncate (new_loc, (base - new_loc->str) - 1);
		//g_print ("new_loc = %s, old_loc = %s\n", new_loc->str, old_loc->str);

		if ((strcmp (new_loc->str, old_loc->str) != 0))// &&
		    //(strlen (new_loc->str) > strlen (old_loc->str)))		// files between dirs in list!
		{
			//g_print ("progress %s\n", new_loc->str);
			g_string_assign (old_loc, new_loc->str);
		}

		g_strfreev (parts);
	}
*/
