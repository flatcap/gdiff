/*
    options.h -- description
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

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <gnome.h>

typedef struct _Options Options;

typedef struct _PrefColours PrefColours;

struct _PrefColours
{
	GdkColor fg;
	GdkColor base;
};

struct _Options
{
	PrefColours	DirStyleSame;
	PrefColours	DirStyleLeft;
	PrefColours	DirStyleRight;
	PrefColours	DirStyleDiff;
	PrefColours	DirStyleError;
	PrefColours	FileStyleLeft;
	PrefColours	FileStyleRight;
	PrefColours	OverviewStyleLeft;
	PrefColours	OverviewStyleRight;

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
	PageDefault = 0,
	PageStyle   = 0,
	PageConfirmation,
	PageView,
	PageTree,
	PageCompare,
	PageDiff,
	PageMDI,
	PageExclusions,

	PageMax
} PrefsPage;

typedef struct _PrefOption PrefOption;

//Options *	options_new (void);
Options *	options_get_default (PrefOption *list);
//void		options_free (Options *options);
GtkWidget *	get_preferences (GtkWindow *parent, PrefsPage page);
void		options_save (Options *opts);
void		show_preferences (GtkWindow *parent, PrefsPage page);
void		read_config_file (Options *options, PrefOption *list);

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

