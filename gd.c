/*
    gd.c -- description
    Copyright (C) 1999  Richard Russon <richard.russon@ait.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* $Revision: 1.12 $ */

#include "config.h"
#include <gnome.h>
#include <locale.h>
#include "args.h"
#include "mdi.h"
#include "global.h"

/*----------------------------------------------------------------------------*/
int main (int argc, char *argv[], char *envv[]);
/*----------------------------------------------------------------------------*/

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

