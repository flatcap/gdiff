#include <stdio.h>
#include <glib.h>

int main (int arc, char *argv[])
{
	g_mem_profile();

	char *buffer = NULL;

	buffer = (char*) g_malloc (32);
	for (int i = 0; i < 32; i++)
	{
		buffer[i] = 'r';
	}

	g_mem_profile();

	//buffer[-1] = 'a';
	//buffer[32] = 'a';

	g_free (buffer);
	//printf ("after free\n");
	//g_free (buffer);
	//buffer[14] = 'c';
	//printf ("after change\n");
	g_mem_profile();

	return 0;
}
