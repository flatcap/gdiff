#include <gnome.h>
#include "diff.h"

DiffOptions *
diffoptions_new (void)
{
	DiffOptions *diff = NULL;

	diff = g_malloc0 (sizeof (DiffOptions));
	if (diff)
	{
		diff->left  = NULL;
		diff->right = NULL;
		diff->root  = NULL;
	}

	return diff;
}

void diffoptions_free (DiffOptions *diff)
{
	if (diff)
	{
		g_free (diff->left);
		g_free (diff->right);
		//node free recursive (diff->root);
	}
}

