gint
gfc_run_pipe (gchar *acmd, gint infd)
{
	gchar **parts;
	gint childpid;
	gint fds[2];
	
	if (pipe (fds))
		return -1;
	
	childpid = fork ();
	if (childpid < 0)
		return -1;
	
	if (childpid){
		close (fds [1]);
		waitpid (childpid, &childpid, 0);
		return fds [0];
	}
	
	parts = g_strsplit (acmd, " ", -1);
	dup2 (fds [1], 1);
	dup2 (fds [1], 2);
	if (fork()) /* Double-forking is good for the (zombified) soul ;-) */
		exit (0);
	else
		execvp (parts [0], parts);
	
	/* ERROR IF REACHED */
	close (0);
	close (1);
	exit (69);
}

