/* $Id$ */

#ifndef _DIFF_H_
#define _DIFF_H_

#include <gnome.h>

typedef struct _DiffOptions DiffOptions;

struct _DiffOptions
{
	gchar *left;				// The left  file/dir relative to path
	gchar *right;				// The right file/dir relative to path

	gboolean dir;				// File or directory?

	GNode *root;
	gpointer compare_stuff;
};

DiffOptions *	diffoptions_new  (void);
void		diffoptions_free (DiffOptions *diff);

#endif // _DIFF_H_

