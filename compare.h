#ifndef _COMPARE_H_
#define _COMPARE_H_

GtkWidget * compare (char *left, char *right);

typedef struct _Compare Compare;

struct _Compare
{
	char *left;
	char *right;

	gpointer data;	// don't know what format, yet...
};

#endif // _COMPARE_H_

