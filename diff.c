/*
    diff.c -- description
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

/* $Revision: 1.7 $ */

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

