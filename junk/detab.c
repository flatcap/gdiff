/* ------------------------------------------- */
/*     DETAB - Convert tabs to blanks          */
/*             Adapted from Software Tools     */
/*             By Kernighan and Plauger        */
/*                                             */
/*             written by Michael Burton       */
/*             Last Update: 14 Jan 1984       */
/* ------------------------------------------- */
/*     USAGE:                                  */
/*             DETAB FROMFILE TOFILE N         */
/*               N is the number of columns    */
/*                 between tab stops           */
/* ------------------------------------------- */
#include "stdio.h"

#define EOS NULL

int 
main (int argc, char *argv[])
{
	static int      col = 1, n, *fd, *td;
	static char     c, *sp;

	if (argc != 4)
	{
		fputs ("Usage: DETAB FROMFILE TOFILE N\007\n", stdout);
		return;
	}
	if ((fd = fopen (argv[1], "r")) == 0)
	{
		fputs (argv[1], stdout);
		fputs (" not found\007\n", stdout);
		return;
	}
	if ((td = fopen (argv[2], "w")) == 0)
	{
		fputs ("Unable to open ", stdout);
		fputs (argv[2], stdout);
		fputs ("\007\n", stdout);
		return;
	}
	n = atoi (argv[3]);
	if (n < 1 || n > 32)
		fputs ("Tabs < 1 or > 32\007\n", stdout);
	while ((c = fgetc (fd)) != EOF)
	{
		switch (c)
		{
		case '\t':
			do
			{
				fputc (' ', td);
				col++;
			}
			while ((col % n) != 0);
			break;
		case '\n':
			fputc ('\n', td);
			col = 1;
			break;
		default:
			fputc (c, td);
			col++;
		}
	}
	fflush (fd);
	fflush (td);
	fclose (fd);
	fclose (td);
}
