#ifndef _ARGS_H_
#define _ARGS_H_

#include "options.h"
#include "diff.h"

gboolean gnome_init_and_parse_args (const char *app_id,
				    const char *app_version,
				    int argc,
				    char *argv[],
				    Options *global,
				    DiffOptions *diff);

#endif // _ARGS_H_

