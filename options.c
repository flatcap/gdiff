#include "options.h"

Options *
options_get_default (void)
{
	// read from the config file
	return g_malloc0 (sizeof (Options));
}

/*
this will be an object representing a diff's options
there will also an 'option' help by the global object (the defaults)

options_new will read from the config file
commit will save to cfg file

who will own the global options? refcount?
*/

