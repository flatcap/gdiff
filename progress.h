/*
    progress.h -- description
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

/* $Revision: 1.4 $ */

#ifndef _PROGRESS_H_
#define _PROGRESS_H_

#include <gnome.h>

typedef struct _Progress Progress;

Progress *	progress_new		(GtkStatusbar *status);
void		progress_free		(Progress *progress);
void		progress_set_text	(Progress *progress, char *text);

struct _Progress
{
	GtkStatusbar *status;
	char *message;
	guint context;
};

#endif // _PROGRESS_H_

