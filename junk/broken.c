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

static gint gfc_run_pipe (gchar *acmd);

int
main (int argc, char *argv[])
{
	char *left  = "data/read1.c";
	char *right = "data/read2.c";
	char *diff  = "diff -q %s %s";
	char *exec = NULL;
	char buffer[1024];
	int diff_fd = -1;

	gnome_init (APP, VER, argc, argv);
	memset (buffer, 0, sizeof (buffer));

	exec = g_strdup_printf (diff, left, right);

	g_print ("exec = '%s'\n", exec);

	//g_print ("child id = %d\n", gnome_execute_shell (NULL, exec));

	diff_fd = gfc_run_pipe (exec);

	//g_print ("read %d bytes\n", read (diff_fd, buffer, sizeof (buffer)));
	g_print ("read '%s'\n", buffer);

	//C close (diff_fd);

	g_free (exec);

	return 0;
}

static gint
gfc_run_pipe (gchar *acmd)
{
	gchar **parts;
	gint childpid;
	gint fds[2];
	
	if (pipe (fds))
		return -1;

	g_print ("pipe %d, %d\n", fds[0], fds[1]);
	
	childpid = fork ();
	if (childpid < 0)
		return -1;
	
	if (childpid){
		//C close (fds [1]);
		waitpid (childpid, &childpid, 0);
		return fds [0];
	}
	
	parts = g_strsplit (acmd, " ", -1);
	dup2 (fds [1], 1);
	dup2 (fds [1], 2);
	if (fds [1] > 2)
		;//C close (fds [1]);
	if (fork()) /* Double-forking is good for the (zombified) soul ;-) */
		exit (0);
	else
		execvp (parts [0], parts);
	
	/* ERROR IF REACHED */
	close (0);
	close (1);
	exit (69);
}

