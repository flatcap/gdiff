/* $Author$ */
/* $Date$ */
/* $Header$ */
/* $Id$ */
/* $Locker$ */
/* $Log$ */
/* $Name$ */
/* $RCSfile$ */
/* $Revision$ */
/* $Source$ */
/* $State$ */

#include "config.h"
#include <gnome.h>
#include <locale.h>
#include "args.h"
#include "mdi.h"
#include "global.h"

int 
main (int argc, char *argv[], char *envv[])
{
	GnomeMDI    *mdi  = NULL;
	DiffOptions *diff = NULL;

	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

	diff = gnome_init_and_parse_args (PACKAGE, VERSION, argc, argv);

	mdi  = mdi_new (PACKAGE, WINNAME);

	g_return_val_if_fail (mdi != NULL, 1);
	global_init (mdi);						/* Needs a GdkWindow to create pixmaps */

	if (diff)
	{
		mdi_add_diff (mdi, diff);				/* Tree will free the diff later */
	}

	gtk_main();

	diffoptions_free (diff);
	global_close();

	return 0;
}

