#include <gnome.h>

int
main (int argc, char *argv[])
{
	char  *path  = "usr/src/linux-2.2.13/fs/ntfs/fs.c";
	char **parts = NULL;

	g_print ("%s\n", path);
	parts = g_strsplit (path, G_DIR_SEPARATOR_S, -1);

	for (; *parts; parts++)
	{
		g_print ("%s\n", *parts);
	}

	g_strfreev (parts);

	return 0;
}
