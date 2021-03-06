/*
    menu.h -- description
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

#ifndef _MENU_H_
#define _MENU_H_

#include <gnome.h>
#include "mdi.h"

void	menu_create		(GnomeMDI *mdi, GnomeApp *app); //XXX can guess app from mdi
void	set_menu_for_view	(MDIDiffChild *child);
void	dump_menu		(GnomeApp *app);
void	menu_create_main	(GnomeMDI *mdi);
void	set_menu_state		(GnomeMDI *mdi);

#endif // _MENU_H_

