#include <stdio.h>

int
main (int argc, char *argv[])
{
	printf ("%2$s, %1$d\n", 42, "forty-two");
	return 0;
}
