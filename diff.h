/*
    diff.h -- description
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

/* $Revision: 1.11 $ */

#ifndef _DIFF_H_
#define _DIFF_H_

#include <gnome.h>

typedef struct _DiffOptions DiffOptions;

typedef enum
{
	Error,					// No idea
	File,					// Two files
	Dir,					// Two directories
	FilePatch,				// A file and a patch					
	DirPatch,				// A directory and a patch
} DiffType;

struct _DiffOptions
{
	gchar *left;				// The left  path
	gchar *right;				// The right path

	// make the above just the root
	gchar *relative;
	gchar *left_root;
	gchar *right_root;

	DiffType type;				// File or directory?

	//XXX GNode *root;
	//XXX gpointer compare_stuff;

	char *last_error;
};

DiffOptions *	diffoptions_new  (void);
void		diffoptions_free (DiffOptions *diff);

#endif // _DIFF_H_

