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
	gboolean     args = FALSE;

	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

	opt  = options_get_default();
	diff = diffoptions_new();

	args = gnome_init_and_parse_args (PACKAGE, VERSION, argc, argv, opt, diff);

	mdi  = mdi_new (PACKAGE, WINNAME);

	global_init (mdi);			// needs a GdkWindow*

	if (args)
	{
		mdi_add_diff (mdi, opt, diff);	// MDI take responsibility for opt and diff???
	}
	else
	{
		//options_free (opt);
		//diffoptions_free (diff);
	}

	gtk_main();
	global_close();
	return 0;
}

