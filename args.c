#include <stdio.h>
#include <popt.h>

int
main (int argc, char *argv[])
{
	char opt;
	char *left = NULL;
	char *right = NULL;
	int i;
	poptContext context;
	struct poptOption options[] =
	{
		{ "left",    'a', POPT_ARG_STRING, &left,  0,   "left!",    "LEFT!" },
		{ "right",   'b', POPT_ARG_STRING, &right, 0,   "right!",   "RIGHT!" },
		{ "up",      'c', 0,               NULL,   'C', "up!"},
		{ "down",    'd', 0,               NULL,   'D', "down!"},
		{ "top",     'e', 0,               NULL,   'E', "top!"},
		{ "bottom",  'f', 0,               NULL,   'F', "bottom!"},
		{ "strange", 'g', 0,               NULL,   'G', "strange!"},
		{ "charm",   'h', 0,               NULL,   'H', "charm!"},
		POPT_AUTOHELP
		{ NULL, 0, 0, NULL, 0 }
	};

	context = poptGetContext (NULL, argc, argv, options, 0);
	
	while ((opt = poptGetNextOpt (context)) != -1)
	{
		printf ("option %c\n", opt);
	}

	printf ("option %s\n", left);
	printf ("option %s\n", right);

	if (opt < -1)
	{
		printf ("bad option: %s: %s\n", poptBadOption (context, POPT_BADOPTION_NOALIAS), poptStrerror (opt));
	}

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
