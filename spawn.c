/* $Revision: 1.13 $ */

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

