/* $Revision: 1.6 $ */

#include <gnome.h>
#include "diff.h"

/*----------------------------------------------------------------------------*/
DiffOptions * diffoptions_new (void);
void diffoptions_free (DiffOptions *diff);
/*----------------------------------------------------------------------------*/

DiffOptions *
diffoptions_new (void)
{
	DiffOptions *diff = NULL;

	diff = g_malloc0 (sizeof (DiffOptions));
	if (diff)
	{
		diff->left       = NULL;	// XXX malloc0 -> not nec
		diff->right      = NULL;
		diff->root       = NULL;
		diff->last_error = NULL;
	}

	return diff;
}

void
diffoptions_free (DiffOptions *diff)
{
	if (diff)
	{
		g_free (diff->left);
		g_free (diff->right);
		//node free recursive (diff->root);
	}
}

