#ifndef _OPTIONS_H_
#define _OPTIONS_H_

typedef struct _Options Options

struct _Options
{
	GdkColor left;
	GdkColor right;
}

Options * options_get_default (void);

#endif // _OPTIONS_H_

