/* $Revision: 1.7 $ */

#ifndef _DIFF_H_
#define _DIFF_H_

#include <gnome.h>

typedef struct _DiffOptions DiffOptions;

typedef enum
{
	Error,					// No idea
	File,					// Two files
	Dir,					// Two directories
	FilePatch,				// A file and a patch					
	DirPatch,				// A directory and a patch
} DiffType;

struct _DiffOptions
{
	gchar *left;				// The left  path
	gchar *right;				// The right path

	DiffType type;				// File or directory?

	GNode *root;
	gpointer compare_stuff;

	//char *last_error;
};

DiffOptions *	diffoptions_new  (void);
void		diffoptions_free (DiffOptions *diff);

#endif // _DIFF_H_

