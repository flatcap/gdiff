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
	FILE *f = NULL;

	gnome_init (APP, VER, argc, argv);
	memset (buffer, 0, sizeof (buffer));

	exec = g_strdup_printf (diff, left, right);

	g_print ("exec = '%s'\n", exec);

	f = run_diff (exec);

	while (fgets (buffer, sizeof (buffer), f))
	{
		g_print ("read '%s'\n", buffer);
	}

	fclose (f);
	g_free (exec);

	return 0;
}

void
close_allfd_except_std(void)
{
	int fd_max;
	int fd;

	fd_max = sysconf(_SC_OPEN_MAX);
	for (fd = STDERR_FILENO+1; fd < fd_max; fd++) {
		close(fd);
	}
}

FILE*
run_diff (char *prog)
{
	int pipe_fds[2];
	//char **argv;
	//const char *ptr;
	//int argc, count;
	char **parts;
	
	if (pipe(pipe_fds) == -1) {
		perror("pipe");
		exit(2);
	}

	switch (fork()) {
	case 0:			/* the child */
		g_print ("child 1\n");
		//setlocale(LC_ALL, "C");
		/* 
		 * redirect standard output and standard error into the pipe
		 */
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1) {
			perror("dup");
			exit(2);
		}
		if (dup2(pipe_fds[1], STDERR_FILENO) == -1) {
			perror("dup");
			exit(2);
		}
		close_allfd_except_std();/*close(pipe_fds[0]) is enough.*/

		/* 
		 * split up args passed in into an argument vector for the execv
		 * system call.  this works for an unlimited number of arguments,
		 * but fails to do any quote processing.  arguments with embedded
		 * spaces will break this.
		 */
		
		parts = g_strsplit (prog, " ", -1);

		execvp(parts[0], parts);
		/* NOT REACHED */
		break;

	case -1:			/* fork error */
		perror("fork");
		exit(2);
		break;

	default:			/* the parent */
		g_print ("parent %d\n", pipe_fds[0]);
		/* 
		 * we must close this in the parent or else the close of the 
		 * writer end of the pipe in the child will not cause an EOF 
		 * condition for the reader
		 */
		close(pipe_fds[1]);
		/* 
		 * return the reader side of the pipe as a stdio stream
		 */
		return (fdopen(pipe_fds[0], "r"));
		/* NOTREACHED */
		break;
	}
	/* NOTREACHED */
	return NULL;
}

