#define ENABLE_NLS 1

#include <stdio.h>
#include <gnome.h>
#include <locale.h>

#define PACKAGE   "langtest"
#define LOCALEDIR "/usr/src/src"

int
main (int argc, char *argv[])
{
	char *message = N_("hello world");

	printf ("locale     = %s\n", setlocale (LC_MESSAGES, ""));

	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

	printf ("message    = %s\n", message);
	printf ("translated = %s\n", _(message));

	return 0;
}

