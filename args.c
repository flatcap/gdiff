#include <gnome.h>
#include <popt.h>
#include "options.h"
#include "diff.h"

/* return a pointer to an options object */
gboolean
gnome_init_and_parse_args (const char *app_id,
			   const char *app_version,
			   int argc,
			   char *argv[],
			   Options *global,
			   DiffOptions *diff)
{
	char opt;
	char *left          = NULL;
	char *right         = NULL;
	char *extra         = NULL;
	poptContext context = NULL;

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
		{ NULL, 0, 0, NULL, 0 }
	};

	gnome_init_with_popt_table (app_id, app_version, argc, argv, options, 0, &context);

	//context = poptGetContext (NULL, argc, argv, options, 0);
	//poptSetOtherOptionHelp (context, "PATH1 PATH2");
	
	while ((opt = poptGetNextOpt (context)) > 0)
	{
		//g_print ("option %c\n", opt);
		//poptPrintUsage (context, stdout, 0);
		//poptPrintHelp (context, stdout, 0);
	}

	if (opt < -1)
	{
		g_print ("bad option: %s: %s\n", poptBadOption (context, POPT_BADOPTION_NOALIAS), poptStrerror (opt));
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
			g_print ("no args!\n");
		}
		else if (extra)
		{
			g_print ("too many options = %s\n", extra);
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

			//g_print ("left  %s\n", left);
			//g_print ("right %s\n", right);

			g_free (diff->left);
			g_free (diff->right);

			diff->left  = g_strdup (left);
			diff->right = g_strdup (right);
		}
	}

	poptFreeContext (context);

	return left && right;
}

