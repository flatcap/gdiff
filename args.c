#include <stdio.h>
#include <popt.h>

int
main (int argc, char *argv[])
{
	char opt;
	char *left = NULL;
	char *right = NULL;
	char *ignore = NULL;
	//int i;
	poptContext context;
	struct poptOption options[] =
	{
#if 0
	diff options
		dir dir		compare two directories
		file file	compare two files
		file dir	compare file and dir/file (same name)
		dir file	compare dir/file and file (same name)
		dir		compare dir and .
		file		compare file and . (dir/file (or file is a patch against .))
#endif
		{ "left",                  'l', POPT_ARG_STRING, &left,   'l', "Path to the first  file or directory",                  "left"   },
		{ "right",                 'r', POPT_ARG_STRING, &right,  'r', "Path to the second file or directory",                  "right"  },
		{ "ignore-case",           0,   0,               NULL,    'i', "Consider upper- and lower-case to be the same.",        NULL     },
		{ "ignore-all-space",      'w', 0,               NULL,    'w', "Ignore all white space.",                               NULL     },
		{ "ignore-space-change",   'b', 0,               NULL,    'b', "Ignore changes in the amount of white space.",          NULL     },
		{ "ignore-blank-lines",    'B', 0,               NULL,    'B', "Ignore changes whose lines are all blank.",             NULL     },
		{ "ignore-matching-lines", 'I', POPT_ARG_STRING, &ignore, 'I', "Ignore changes whose lines all match RE.",              "RE"     },
		{ "minimal",               'd', 0,               NULL,    'd', "Try hard to find a smaller set of changes.",            NULL     },
		{ "speed-large-files",     'H', 0,               NULL,    'H', "Assume large files and many scattered small changes.",  NULL     },
		{ "text",                  'a', 0,               NULL,    'a', "Treat all files as text.",                              NULL     },

		POPT_AUTOHELP
		{ NULL, 0, 0, NULL, 0 }
	};

	context = poptGetContext (NULL, argc, argv, options, 0);
	
	while ((opt = poptGetNextOpt (context)) > 0)
	{
		printf ("option %c\n", opt);
	}

	printf ("option %s\n", left);
	printf ("option %s\n", right);

	if (opt < -1)
	{
		printf ("bad option: %s: %s\n", poptBadOption (context, POPT_BADOPTION_NOALIAS), poptStrerror (opt));
	}

#if 0
	for (i = 0; options[i].longName; i++)
	{
		printf ("%-10s %c %d %-4d %d %s %s\n",
			options[i].longName,
			options[i].shortName,
			options[i].argInfo,
			options[i].arg ? *((int*)(options[i].arg)) : 0,
			options[i].val,
			options[i].descrip,
			options[i].argDescrip);
	}
#endif

	printf ("arg = %s\n", poptGetArg (context));
	printf ("arg = %s\n", poptGetArg (context));
	printf ("arg = %s\n", poptGetArg (context));

#if 0
	struct poptOption {
	    const char * longName;	/* may be NULL */
	    char shortName;		/* may be '\0' */
	    int argInfo;
	    void * arg;			/* depends on argInfo */
	    int val;			/* 0 means don't return, just update flag */
	    char * descrip;		/* description for autohelp -- may be NULL */
	    char * argDescrip;		/* argument description for autohelp */
	};
#endif

	//poptPrintUsage (context, stdout, 0);
	//poptPrintHelp (context, stdout, 0);

	poptFreeContext (context);
	return 0;
}
