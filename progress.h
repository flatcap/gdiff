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

