/* $Revision: 1.6 $ */

#include "options.h"

/*
this will be an object representing a diff's options
there will also an 'option' help by the global object (the defaults)

options_new will read from the config file
commit will save to cfg file

who will own the global options? refcount?
*/

/*----------------------------------------------------------------------------*/
Options * options_get_default (void);
Options * options_new (void);
void options_free (Options *options);
gboolean options_set_options  (Options *options);
gboolean options_save_options (Options *options);
/*----------------------------------------------------------------------------*/

Options *
options_get_default (void)
{
	// ask global for this first
	// read from the config file
	return g_malloc0 (sizeof (Options));
}

Options *
options_new (void)
{
	return NULL;
}

void
options_free (Options *options)
{
	g_free (options);
}

gboolean
options_set_options  (Options *options)
{
	return FALSE;
}

gboolean
options_save_options (Options *options)
{
	gboolean result = FALSE;

	if (options_set_options (options))
	{
		//save to config file
	}

	return result;
}

