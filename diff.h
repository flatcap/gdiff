#ifndef _DIFF_H_
#define _DIFF_H_

#include <gnome.h>

typedef struct _DiffOptions DiffOptions;

struct _DiffOptions
{
	gchar *left;
	gchar *right;

	GNode *root;
};

DiffOptions * diffoptions_new (void);

#endif // _DIFF_H_

