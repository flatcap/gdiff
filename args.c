#include <stdio.h>
#include <popt.h>

/* return a pointer to an options object */
void
parse_args (int argc, char *argv[])
{
	char opt;
	char *left   = NULL;
	char *right  = NULL;
	char *extra  = NULL;
	poptContext context;
	struct poptOption options[] =
	{
		{ "left",                  'l', POPT_ARG_STRING, &left,   'l', "Path to the first  file or directory",                    "left"   },
		{ "right",                 'r', POPT_ARG_STRING, &right,  'r', "Path to the second file or directory",                    "right"  },
		//{ "ignore-case",           0,   0,               NULL,    'i', "Consider upper- and lower-case to be the same.",          NULL     },
		//{ "ignore-all-space",      'w', 0,               NULL,    'w', "Ignore all white space.",                                 NULL     },
		//{ "ignore-space-change",   'b', 0,               NULL,    'b', "Ignore changes in the amount of white space.",            NULL     },
		//{ "ignore-blank-lines",    'B', 0,               NULL,    'B', "Ignore changes whose lines are all blank.",               NULL     },
		//{ "ignore-matching-lines", 'I', POPT_ARG_STRING, &ignore, 'I', "Ignore changes whose lines all match RE.",                "RE"     },
		//{ "minimal",               'd', 0,               NULL,    'd', "Try hard to find a smaller set of changes.",              NULL     },
		//{ "speed-large-files",     'H', 0,               NULL,    'H', "Assume large files and many scattered small changes.",    NULL     },
		//{ "text",                  'a', 0,               NULL,    'a', "Treat all files as text.",                                NULL     },
		//{ "set-defaults",          0,   0,               NULL,    's', "Make the selected options the default for future usage.", NULL     },
		POPT_AUTOHELP
		{ NULL, 0, 0, NULL, 0 }
	};

	context = poptGetContext (NULL, argc, argv, options, 0);
	//poptSetOtherOptionHelp (context, "PATH1 PATH2");
	
	while ((opt = poptGetNextOpt (context)) > 0)
	{
		//printf ("option %c\n", opt);
		//poptPrintUsage (context, stdout, 0);
		//poptPrintHelp (context, stdout, 0);
	}

	if (opt < -1)
	{
		printf ("bad option: %s: %s\n", poptBadOption (context, POPT_BADOPTION_NOALIAS), poptStrerror (opt));
	}
	else
	{
		while ((extra = poptGetArg (context)))
		{
			if (!left)
			{
				left = extra;
			}
			else if (!right)
			{
				right = extra;
			}
			else
			{
				break;
			}
		}

		if (!left && !right)
		{
			printf ("no args!\n");
		}
		else if (extra)
		{
			printf ("too many options = %s\n", extra);
		}
		else
		{
			if (!right)
			{
				right = ".";
			}
			if (!left)
			{
				left = ".";
			}

			printf ("left  %s\n", left);
			printf ("right %s\n", right);
		}
	}

	poptFreeContext (context);
}

int
main (int argc, char *argv[])
{
	parse_args (argc, argv);

	return 0;
}

