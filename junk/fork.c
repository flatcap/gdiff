#include <stdio.h>
#include <unistd.h>

int
main (int argc, char *argv[])
{
	switch (fork())
	{
	case 0:		printf ("child\n");
			break;

	case -1:	printf ("error\n");
			break;

	default:	printf ("parent\n");
			break;
	}

	return 0;
}

