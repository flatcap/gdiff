/*
    args.c -- description
    Copyright (C) 1999  Richard Russon <richard.russon@ait.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Revision: 1.25 $ */

#include <gnome.h>
#include <popt.h>
#include <unistd.h>
#include <sys/stat.h>
#include "args.h"
#include "options.h"
#include "diff.h"

/*----------------------------------------------------------------------------*/
static DiffType	identify_path   (char *path);
static void	swap_strings    (char **a, char **b);

char *		standardise_arg (char *path);
void		categorise_args (DiffOptions *diff);
DiffOptions *	gnome_init_and_parse_args (const char *app_id, const char *app_version, int argc, char *argv[]);
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

				//g_print ("type  = %s\n", (diff->type == File) ? "file" : "dir");
				//g_print ("left  = %s\n", diff->left);
				//g_print ("right = %s\n", diff->right);
			}
		}
	}

	poptFreeContext (context);

	return diff;
}

DiffType
identify_path (char *path)
{
	struct stat info;

	g_return_val_if_fail (path != NULL, Error);

	if (stat (path, &info) == 0)
	{
		if      (S_ISREG (info.st_mode))  return File;
		else if (S_ISDIR (info.st_mode))  return Dir;	/* We can't use anything else */
	}

	return Error;
}

void
swap_strings (char **a, char **b)
{
	char *c = *a;
	*a = *b;
	*b = c;
}

char *
standardise_arg (char *path)
{
	GString *temp   = NULL;
	char    *result = NULL;
	int      len    = 0;

	g_return_val_if_fail (path != NULL, NULL);

	temp = g_string_new (path);
	g_free (path);

	len = strlen (temp->str);
	if (len > 0)
	{
		while (temp->str[len-1] == G_DIR_SEPARATOR)
		{
			len--;
			g_string_truncate (temp, len);
		}
	}

	result = temp->str;
	g_string_free (temp, FALSE);

	return result;
}

void
categorise_args (DiffOptions *diff)
{
	gboolean  reversed = FALSE;
	DiffType  ltype    = Error;
	DiffType  rtype    = Error;
	char     *path     = NULL;

	g_return_if_fail (diff        != NULL);
	g_return_if_fail (diff->left  != NULL);
	g_return_if_fail (diff->right != NULL);

	diff->left  = standardise_arg (diff->left);
	diff->right = standardise_arg (diff->right);

	ltype = identify_path (diff->left);
	rtype = identify_path (diff->right);

	diff->type = Error;

	if (ltype == Error)
	{
		diff->last_error = "left path is a non-file";
		return;
	}
	else if (rtype == Error)
	{
		diff->last_error = "right path is a non-file";
		return;
	}

	if (ltype == rtype)
	{
		diff->type = ltype;				// can only be File or Dir
		return;
	}

	if (ltype == Dir)					// We must have one file and one dir
	{
		swap_strings (&diff->left, &diff->right);	// This seems strange, but it simplifies
		reversed = TRUE;				// the logic (below) immensely
	}							// left == file, right == dir

	path = g_strjoin (G_DIR_SEPARATOR_S, diff->right, g_basename (diff->left), NULL);

	switch (identify_path (path))
	{
	case File:	g_free (diff->right);
			diff->right = g_strdup (path);
			diff->type = File;
			break;

	case Dir:	diff->last_error = "args are different types";
			break;
	
	default:	// see if 'file' is a patch
			// see if 'dir' is an RCS / CVS dir
			diff->last_error = "path %s doesn't exist", path;
			break;
	}

	if (reversed)
	{
		swap_strings (&diff->left, &diff->right);
	}

	g_free (path);
}

