/* $Revision: 1.9 $ */

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <gnome.h>

typedef struct _Options Options;

struct _Options
{
	GtkStyle	*DirStyleSame;
	GtkStyle	*DirStyleLeft;
	GtkStyle	*DirStyleRight;
	GtkStyle	*DirStyleDiff;
	GtkStyle	*DirStyleError;
	GtkStyle	*FileStyleLeft;
	GtkStyle	*FileStyleRight;

	guint		ConfirmOnClose;
	guint		ConfirmOnExit;
	guint		ConfirmDirMeansAllFiles;
	guint		AutoBrowseOnOpen;

	guint		DirViewSame;
	guint		DirViewLeft;
	guint		DirViewRight;
	guint		DirViewDiff;
	guint		DirViewError;

	guint		DirViewStyle;
	guint		DirStatistics;
	guint		DirFileCount;

	guint		FilePaneStyle;
	guint		FileScrollbar;
	guint		FileOverview;
	guint		FileLineNumbers;

	guint		DiffIgnoreCase;
	guint		DiffIgnoreAllSpace;
	guint		DiffIgnoreSpaceChange;
	guint		DiffIgnoreBlankLines;
	guint		DiffMinimal;
	guint		DiffSmallChanges;
	guint		DiffTreatAsText;

	guint		MDIStyle;
	guint		SessionStyle;

	gchar		*Exclusions;
};

typedef enum
{
	Default = 0,
	Style   = 0,
	Confirmation,
	View,
	Tree,
	Compare,
	Diff,
	MDI,
	Exclusions,

	MaxPage
} PrefsPage;

typedef struct _PrefOption PrefOption;

//Options *	options_new (void);
Options *	options_get_default (PrefOption *list);
//void		options_free (Options *options);
GtkWidget *	get_preferences (GtkWindow *parent, PrefsPage page);
void		options_save (Options *opts);

//gboolean	options_set_options  (Options *options);
//gboolean	options_save_options (Options *options);

// one callback for each distinct type
// notify ancestor (of type GnomePropertyBox) of changes
typedef enum
{
	PrefPage = 1,
	PrefFrame,
	PrefCheck,
	PrefLabel,
	PrefRadio,
	PrefStyle,
	PrefList,

	PrefMax
} PrefType;

struct _PrefOption
{
	PrefType type;
	char *label;
	char *rcfile;
	guint offset;
};

#endif // _OPTIONS_H_

