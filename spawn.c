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
	char buffer[1024];
	char *exec = "diff data/read1.c data/read2.c";
	char *clean = NULL;
	FILE *file = NULL;

	gnome_init (APP, VER, argc, argv);

	file = run_diff (exec);
	while (fgets (buffer, sizeof (buffer), file))
	{
		clean = g_strchomp (g_strdup (buffer));
		g_print ("read '%s'\n", clean);
		g_free (clean);
	}

	fclose (file);
	return 0;
}

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

