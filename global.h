/* $Id$ */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <gnome.h>
#include <regex.h>

extern GdkPixmap *pixmap_open;
extern GdkPixmap *pixmap_closed;
extern GdkPixmap *pixmap_leaf;

extern GdkBitmap *mask_open;
extern GdkBitmap *mask_closed;
extern GdkBitmap *mask_leaf;

#define MATCHES 8

extern regex_t    reg_same;
extern regex_t    reg_diff;
extern regex_t    reg_only;
extern regex_t    reg_type;

gboolean global_init  (GnomeMDI *mdi);
void     global_close (void);

//private stuff
//parse config file

#endif // _GLOBAL_H_

