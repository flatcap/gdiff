#ifndef _PROGRESS_H_
#define _PROGRESS_H_

#include <gnome.h>

GtkWidget *	progress_new		(void);
void		progress_set_text	(GtkWidget *progress, char *text);

#endif // _PROGRESS_H_

