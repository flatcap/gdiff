/*
    spawn.c -- description
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

/* $Revision: 1.14 $ */

#include <stdio.h>
#include <gnome.h>
#include <sys/wait.h>
#include "spawn.h"

/*----------------------------------------------------------------------------*/
FILE * run_diff (gchar *prog);
/*----------------------------------------------------------------------------*/

FILE *
run_diff (gchar *prog)
{
	gint    fds[2] = { -1, -1 };
	gint    pid    = -1;
	gchar **parts  = NULL;
	FILE   *file   = NULL;

	if (!pipe (fds))
	{
		pid = fork();
		if (pid == 0)			/* Child */
		{
			dup2 (fds[1], STDOUT_FILENO);
			dup2 (fds[1], STDERR_FILENO);
			close (fds[0]);

			parts = g_strsplit (prog, " ", -1);

			execvp (parts[0], parts);
			g_assert_not_reached();
		}
		else if (pid > 0)		/* Parent */
		{
			close (fds[1]);
			file = fdopen (fds[0], "r");
		}
	}

	return file;
}

