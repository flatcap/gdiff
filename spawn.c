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

#include <stdio.h>
#include <gnome.h>
#include <sys/wait.h>
#include "spawn.h"

/*----------------------------------------------------------------------------*/
FILE * run_diff (gchar *prog);
/*----------------------------------------------------------------------------*/

FILE *
run_diff2 (gchar *prog)
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

			//if (fork())
			//	exit (0);
			//else
				execvp (parts[0], parts);
			g_assert_not_reached();
		}
		else if (pid > 0)		/* Parent */
		{
			close (fds[1]);
			//waitpid (pid, NULL, 0);
			file = fdopen (fds[0], "r");
		}
	}

	return file;
}

//XXX close_program does waitpid too

FILE *
run_diff (gchar *acmd)
{
	gchar **parts;
	gint childpid;
	gint fds[2];

	if (pipe (fds))
		return NULL;

	childpid = fork ();
	if (childpid < 0)
		return NULL;

	if (childpid){
		close (fds [1]);
		//waitpid (childpid, &childpid, 0);
		waitpid (-1, NULL, WNOHANG);
		return fdopen (fds [0], "r");
	}

	parts = g_strsplit (acmd, " ", -1);
	dup2 (fds [1], 1);
	dup2 (fds [1], 2);
	//if (fork()) /* Double-forking is good for the (zombified) soul ;-) */
	//	exit (0);
	//else
		execvp (parts [0], parts);

	/* ERROR IF REACHED */
	close (0);
	close (1);
	exit (69);
}

