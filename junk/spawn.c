/*
 * This file is derived from mgdiff,
 * and modified by INOUE Seiichiro <inoue@ainet.or.jp>.
 *
 * #ifndef lint
 * static char rcsid[] = "spawn.c,v 2.0 1994/05/19 02:01:23 dan Exp";
 * #endif
*/
/*
 * Copyright (c) 1994    Daniel Williams
 * 
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge,
 * a full and unrestricted irrevocable, world-wide, paid up,
 * royalty-free, nonexclusive right and license to deal in this software
 * and documentation files (the "Software"), including without limitation
 * the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons
 * who receive copies from any such party to do so.  This license
 * includes without limitation a license to do the foregoing actions
 * under any patents of the party supplying this software to the X
 * Consortium.  The following conditions apply:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL DANIEL WILLIAMS OR SYSTEMS & SCIENTIFIC SOFTWARE BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#if defined(HAVE_STRING_H)
#include <string.h>
#elif defined(HAVE_STRINGS_H)
#include <strings.h>
#endif
#include <unistd.h>
#include <locale.h>
#if defined(HAVE_ERRNO_H)
#include <errno.h>
#else defined(HAVE_SYS_ERRNO_H)
#include <sys/errno.h>
#endif
#include "misc.h"

#define BLOCKSIZE 10

/* 
 * run a program with command line arguments and two pathname 
 * arguments via fork/exec and return a pipe file descriptor into 
 * which standard output and standard error have been redirected
 */
FILE*
spawn_diff(char *prog, char *args, char *path1, char *path2)
{
	int pipe_fds[2];
	char **argv;
	const char *ptr;
	int argc, count;
#if defined(HAVE_STRTOK_R)
	char *last;
#endif
	
	if (pipe(pipe_fds) == -1) {
		perror("pipe");
		exit(2);
	}

	switch (fork()) {
	case 0:			/* the child */
		setlocale(LC_ALL, "C");
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
		argc = 0;
		count = BLOCKSIZE;
		argv = (char **)malloc(sizeof(char *) * BLOCKSIZE);
		argv[argc++] = prog;
		
#if defined(HAVE_STRSEP)
		for (ptr = strsep(&args, " \t"); ptr; ptr = strsep(&args, " \t")) {
#elif defined(HAVE_STRTOK_R)
		for (ptr = strtok_r(args, " \t", &last); ptr; ptr = strtok_r(last, " \t", &last)) {
#else
		for (ptr = strtok(args, " \t"); ptr; ptr = strtok(NULL, " \t")) {
#endif			
			if (argc >= count) {
				argv = (char **)realloc(argv, sizeof (char *) * BLOCKSIZE);
				count += BLOCKSIZE;
			}
			argv[argc++] = strdup(ptr);
		}
		
		if ((argc + 3) >= count)
			argv = (char **)realloc(argv, sizeof (char *) * 3);
		
		argv[argc++] = path1;
		argv[argc++] = path2;
		argv[argc++] = NULL;
		
		if (execv(prog, argv) == -1) {
			exit(2);
		}

		break;

	case -1:			/* fork error */
		perror("fork");
		exit(2);
		break;

	default:			/* the parent */
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

