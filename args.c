#include <stdio.h>
#include <popt.h>

int
main (int argc, char *argv[])
{
	poptContext context;
	static struct poptOption options[] =
	{
		{ "left",    'a', 0, NULL, 0, "left!",    "LEFT!" },
		{ "right",   'b', 0, NULL, 0, "right!",   "RIGHT!" },
		{ "up",      'c', 0, NULL, 0, "up!",      "UP!" },
		{ "down",    'd', 0, NULL, 0, "down!",    "DOWN!" },
		{ "top",     'e', 0, NULL, 0, "top!",     "TOP!" },
		{ "bottom",  'f', 0, NULL, 0, "bottom!",  "BOTTOM!" },
		{ "strange", 'g', 0, NULL, 0, "strange!", "STRANGE!" },
		{ "charm",   'h', 0, NULL, 0, "charm!",   "CHARM!" },
		POPT_AUTOHELP
		{ NULL, 0, 0, NULL, 0 }
	};

	context = poptGetContext (NULL, argc, argv, options, 0);
	
	poptPrintUsage (context, stdout, 0);

	return 0;
}
