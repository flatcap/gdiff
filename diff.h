#ifndef _DIFF_H_
#define _DIFF_H_

#include <gnome.h>

typedef struct _DiffOptions DiffOptions;

struct _DiffOptions
{
	gchar *left;
	gchar *right;

	GNode *root;
	//----------
	char *path; // from which left and right are relative
	gboolean dir; // file or directory?
	gpointer compare_stuff;
};

DiffOptions *	diffoptions_new  (void);
void		diffoptions_free (DiffOptions *diff);

#endif // _DIFF_H_

