#include <stdio.h>
#include <gnome.h>
#include "spawn.h"

#include <sys/wait.h>

#define APP "spawn"
#define VER "0.0.1"
#define TTL "Spawn"

FILE * spawn_diff (char *args, char *left, char *right)
{
	return stdin;
}

FILE * run_diff (char *prog);

int
main (int argc, char *argv[])
{
	char *left  = "data/read1.c";
	char *right = "data/read2.c";
	char *diff  = "diff %s %s";
	char *exec = NULL;
	char buffer[1024];
	char *clean = NULL;
	FILE *file = NULL;

	gnome_init (APP, VER, argc, argv);
	memset (buffer, 0, sizeof (buffer));

	exec = g_strdup_printf (diff, left, right);

	g_print ("exec = '%s'\n", exec);

	file = run_diff (exec);

	while (fgets (buffer, sizeof (buffer), file))
	{
		clean = g_strchomp (g_strdup (buffer));
		g_print ("read '%s'\n", clean);
		g_free (clean);
	}

	fclose (file);
	g_free (exec);

	return 0;
}

FILE *
run_diff (char *prog)
{
	int     fds[2] = { -1, -1 };
	int     pid    = -1;
	char  **parts  = NULL;
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
			g_assert_not_reached ();
		}
		else if (pid > 0)		/* Parent */
		{
			close (fds[1]);
			file = fdopen (fds[0], "r");
		}
	}

	return file;
}
