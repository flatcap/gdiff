/*
    global.h -- description
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

/* $Revision: 1.5 $ */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <gnome.h>
#include <regex.h>
#include "options.h"

extern GdkPixmap *pixmap_open;
extern GdkPixmap *pixmap_closed;
extern GdkPixmap *pixmap_leaf;

extern GdkBitmap *mask_open;
extern GdkBitmap *mask_closed;
extern GdkBitmap *mask_leaf;

extern regex_t    reg_same;
extern regex_t    reg_diff;
extern regex_t    reg_only;
extern regex_t    reg_type;

#define MATCHES 8

gboolean  global_init        (GnomeMDI *mdi);
void      global_close       (void);
Options  *global_get_options (PrefOption *list);

#endif // _GLOBAL_H_

