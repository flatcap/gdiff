/* $Revision: 1.17 $ */

#include <gnome.h>
#include <popt.h>
#include <unistd.h>
#include <sys/stat.h>
#include "args.h"
#include "options.h"
#include "diff.h"

/*----------------------------------------------------------------------------*/
DiffOptions * gnome_init_and_parse_args (const char *app_id, const char *app_version, int argc, char *argv[]);
/*----------------------------------------------------------------------------*/

/* return a pointer to an options object */
DiffOptions *
gnome_init_and_parse_args (const char *app_id,
			   const char *app_version,
			   int argc,
			   char *argv[])
{
	char opt;
	char *left          = NULL;
	char *right         = NULL;
	char *extra         = NULL;
	DiffOptions *diff   = NULL;
	poptContext context = NULL;
	//Options     *global = NULL;

	//global  = options_get_default();

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
			//g_print ("no args!\n");
		}
		else if (extra)
		{
			g_warning ("too many options = %s\n", extra);
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

			diff = diffoptions_new();
			if (diff)
			{
				diff->left  = g_strdup (left);
				diff->right = g_strdup (right);
				categorise_args (diff);
			}
		}
	}

	poptFreeContext (context);

	return diff;
}

gboolean
categorise_args (DiffOptions *diff)
{
	//gboolean reversed = FALSE;
	struct stat lstat;
	struct stat rstat;
	struct stat pstat;
	DiffType ltype = Error;
	DiffType rtype = Error;
	DiffType ptype = Error;
	char *dir = NULL;
	char *file = NULL;
	char *path = NULL;
	char *base = NULL;

	g_return_val_if_fail (diff        != NULL, FALSE);
	g_return_val_if_fail (diff->left  != NULL, FALSE);
	g_return_val_if_fail (diff->right != NULL, FALSE);

	if (stat (diff->left, &lstat))
	{
		g_print ("left file bad\n");
		return FALSE;
	}

	if (stat (diff->right, &rstat))
	{
		g_print ("right file bad\n");
		return FALSE;
	}

	// Preliminary
	if      (S_ISREG (lstat.st_mode))	ltype = File;
	else if (S_ISDIR (lstat.st_mode))	ltype = Dir;

	if      (S_ISREG (rstat.st_mode))	rtype = File;
	else if (S_ISDIR (rstat.st_mode))	rtype = Dir;

	if (ltype == Error)
	{
		g_print ("left path is a non-file\n");
		return FALSE;
	}
	else if (rtype == Error)
	{
		g_print ("right path is a non-file\n");
		return FALSE;
	}

	if (ltype == rtype)
	{
		if (ltype == File)
		{
			diff->type = File;
		}
		else
		{
			diff->type = Dir;
		}
	}
	else
	{
		dir  = (ltype == Dir)  ? diff->left : diff->right;
		file = (ltype == File) ? diff->left : diff->right;

		g_print ("file = %s\n", file);
		g_print ("dir  = %s\n", dir);

		base = g_basename (file);
		if (!base)
			base = file;

		path = g_strjoin (G_DIR_SEPARATOR_S, dir, base, NULL);

		g_print ("path = %s\n", path);

		if (stat (path, &pstat) == 0)
		{
			if      (S_ISREG (pstat.st_mode))	ptype = File;
			else if (S_ISDIR (pstat.st_mode))	ptype = Dir;

			if (((ltype == File) && (ptype == File)) ||
			    ((rtype == File) && (ptype == File)))
			{
				if (ltype == Dir)
				{
					g_free (diff->left);
					diff->left = path;
				}
				else
				{
					g_free (diff->right);
					diff->right = path;
				}
				diff->type = File;
			}
			else
			{
				g_print ("args are different types\n");
				g_free (path);
				return FALSE;
			}
		}
		else
		{
			g_print ("path %s doesn't exist\n", path);
			// see if 'file' is a patch
			// see if 'dir' is an RCS / CVS dir
			g_free (path);
			return FALSE;
		}
	}

	g_print ("left  = %s, %d\n", diff->left,  ltype);
	g_print ("right = %s, %d\n", diff->right, rtype);
	g_print ("type  = %s\n",     (diff->type == File) ? "file" : "dir");

	return TRUE;
}

