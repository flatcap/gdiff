#include "config.h"

#include <gnome.h>
#include <locale.h>
#include "args.h"
#include "mdi.h"
#include "global.h"

#define WINNAME "Graphical differences"

int 
main (int argc, char *argv[], char *envv[])
{
	GnomeMDI    *mdi  = NULL;
	Options     *opt  = NULL;
	DiffOptions *diff = NULL;

	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

	//gnome_init (PACKAGE, VERSION, argc, argv);

	opt  = options_get_default();
	diff = diffoptions_new();

	gnome_init_and_parse_args (PACKAGE, VERSION, argc, argv, opt, diff);

	mdi  = mdi_new (PACKAGE, WINNAME);

	global_init (mdi);			// needs a GdkWindow*

	mdi_add_diff (mdi, opt, diff);
	//mdi_add_diff (mdi, opt, diff);
	//mdi_add_diff (mdi, opt, diff);

	//g_free (opt);	 // maybe?	// who's going to own these?
	//g_free (diff);

	mdi_show_all (mdi);
	gtk_main();

	//gnome_mdi_remove_all (mdi, FALSE);
	global_close();
	
	return 0;
}

