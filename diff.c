#include <gnome.h>
#include "diff.h"

DiffOptions *
diffoptions_new (void)
{
	return g_malloc0 (sizeof (DiffOptions));
}

