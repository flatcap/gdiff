/*
    tree.h -- description
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

/* $Revision: 1.10 $ */

#ifndef _TREE_H_
#define _TREE_H_

#include <gtk/gtk.h>
#include "node.h"
#include "derived.h"

//GtkDiffTree *	tree_dialog_new   (GtkWidget *parent);
gboolean	tree_dialog_parse (GtkDiffTree *tree, char *left, char *right);
gboolean	tree_dialog_draw  (GtkDiffTree *tree, Status status);
//void		tree_dialog_free  (GtkDiffTree *tree);


#if 0
	construct with base_left, base_right, return a widget

	set_view_status -- change visibility of nodes

	get color and font info from global -- callbacks into main? ;
	construct the tree with a ptr to a config engine?

	by deriving a new tree we can override all the allocation routines
	when finished unref or free? -- MUST be unref to make sure the base
	classes are freed cleanly
#endif

#endif // _TREE_H_

