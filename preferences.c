/*
    preferences.c -- description
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

//XXX Add an enumeration to the radio entries -- display the buttons in enum order

PrefOption options_list[] =
{
	{ PrefPage,	N_("Style"),					"Style" },
	{ PrefFrame,	N_("Directory") },
	{ PrefStyle,	N_("Same"),					"DirStyleSame",			G_STRUCT_OFFSET (Options, DirStyleSame) },
	{ PrefStyle,	N_("Left"),					"DirStyleLeft",			G_STRUCT_OFFSET (Options, DirStyleLeft) },
	{ PrefStyle,	N_("Right"),					"DirStyleRight",		G_STRUCT_OFFSET (Options, DirStyleRight) },
	{ PrefStyle,	N_("Diff"),					"DirStyleDiff",			G_STRUCT_OFFSET (Options, DirStyleDiff) },
	{ PrefStyle,	N_("Error / Type"),				"DirStyleError",		G_STRUCT_OFFSET (Options, DirStyleError) },
	{ PrefFrame,	N_("File") },
	{ PrefStyle,	N_("Left"),					"FileStyleLeft",		G_STRUCT_OFFSET (Options, FileStyleLeft) },
	{ PrefStyle,	N_("Right"),					"FileStyleRight",		G_STRUCT_OFFSET (Options, FileStyleRight) },

	{ PrefPage,	N_("Confirmation"),				"Confirmation" },
	{ PrefFrame,	N_("Confirmation") },
	{ PrefCheck,	N_("Confirm on close view"),			"ConfirmOnClose",		G_STRUCT_OFFSET (Options, ConfirmOnClose) },
	{ PrefCheck,	N_("Confirm on exit"),				"ConfirmOnExit",		G_STRUCT_OFFSET (Options, ConfirmOnExit) },
	{ PrefCheck,	N_("Close directory implies close all files"),	"ConfirmDirMeansAllFiles",	G_STRUCT_OFFSET (Options, ConfirmDirMeansAllFiles) },
	{ PrefCheck,	N_("Automatically browse on open"),		"AutoBrowseOnOpen",		G_STRUCT_OFFSET (Options, AutoBrowseOnOpen) },

	{ PrefPage,	N_("View"),					"View" },
	{ PrefFrame,	N_("View") },
	{ PrefCheck,	N_("Same"),					"DirViewSame",			G_STRUCT_OFFSET (Options, DirViewSame) },
	{ PrefCheck,	N_("Left"),					"DirViewLeft",			G_STRUCT_OFFSET (Options, DirViewLeft) },
	{ PrefCheck,	N_("Right"),					"DirViewRight",			G_STRUCT_OFFSET (Options, DirViewRight) },
	{ PrefCheck,	N_("Diff"),					"DirViewDiff",			G_STRUCT_OFFSET (Options, DirViewDiff) },
	{ PrefCheck,	N_("Error / Type"),				"DirViewError",			G_STRUCT_OFFSET (Options, DirViewError) },

	{ PrefPage,	N_("Directory"),				"Directory" },
	{ PrefFrame,	N_("Display style") },
	{ PrefRadio,	N_("Tree"),					"DirViewStyle",			G_STRUCT_OFFSET (Options, DirViewStyle) },				// Tree,				list (path),				list (no path)
	{ PrefRadio,	N_("List of files (full path)") },
	{ PrefRadio,	N_("List of files (no path)") },
	{ PrefFrame,	N_("Directory options") },
	{ PrefCheck,	N_("Statistics (very slow)"),			"DirStatistics",		G_STRUCT_OFFSET (Options, DirStatistics) },
	{ PrefCheck,	N_("File count"),				"DirFileCount",			G_STRUCT_OFFSET (Options, DirFileCount) },

	{ PrefPage,	N_("File"),					"File" },
	{ PrefFrame,	N_("Pane style") },
	{ PrefRadio,	N_("One pane"),					"FilePaneStyle",		G_STRUCT_OFFSET (Options, FilePaneStyle) },
	{ PrefRadio,	N_("Two panes") },
	{ PrefFrame,	N_("File options") },
	{ PrefCheck,	N_("Scrollbar"),				"FileScrollbar",		G_STRUCT_OFFSET (Options, FileScrollbar) },
	{ PrefCheck,	N_("Overview"),					"FileOverview",			G_STRUCT_OFFSET (Options, FileOverview) },
	{ PrefCheck,	N_("Line numbers"),				"FileLineNumbers",		G_STRUCT_OFFSET (Options, FileLineNumbers) },

	{ PrefPage,	N_("Diff"),					"Diff" },
	{ PrefFrame,	N_("Diff options") },
	{ PrefCheck,	N_("Ignore case"),				"DiffIgnoreCase",		G_STRUCT_OFFSET (Options, DiffIgnoreCase) },
	{ PrefCheck,	N_("Ignore all space"),				"DiffIgnoreAllSpace",		G_STRUCT_OFFSET (Options, DiffIgnoreAllSpace) },
	{ PrefCheck,	N_("Ignore space change"),			"DiffIgnoreSpaceChange",	G_STRUCT_OFFSET (Options, DiffIgnoreSpaceChange) },
	{ PrefCheck,	N_("Ignore blank lines"),			"DiffIgnoreBlankLines",		G_STRUCT_OFFSET (Options, DiffIgnoreBlankLines) },
	{ PrefCheck,	N_("Minimal"),					"DiffMinimal",			G_STRUCT_OFFSET (Options, DiffMinimal) },
	{ PrefCheck,	N_("Small changes in a large file"),		"DiffSmallChanges",		G_STRUCT_OFFSET (Options, DiffSmallChanges) },
	{ PrefCheck,	N_("Treat as text"),				"DiffTreatAsText",		G_STRUCT_OFFSET (Options, DiffTreatAsText) },

	{ PrefPage,	N_("MDI"),					"MDI" },
	{ PrefFrame,	N_("MDI style") },
	{ PrefRadio,	N_("Default"),					"MDIStyle",			G_STRUCT_OFFSET (Options, MDIStyle) },				// Default,				Top Level,				Modal, Notebook
	{ PrefRadio,	N_("Top Level") },
	{ PrefRadio,	N_("Modal") },
	{ PrefRadio,	N_("Notebook") },
	{ PrefFrame,	N_("Session Management") },
	{ PrefRadio,	N_("Open nothing"),				"SessionStyle",			G_STRUCT_OFFSET (Options, SessionStyle) },				// Open nothing,				Open,				but require rescan,				Rescan everything
	{ PrefRadio,	N_("Open, but require rescan") },
	{ PrefRadio,	N_("Rescan everything") },

	{ PrefPage,	N_("Exclusions"),				"Exclusions" },
	{ PrefFrame,	N_("Exclude files matching") },
	{ PrefList,	NULL,						"Exclusions",			G_STRUCT_OFFSET (Options, Exclusions) },

	{ 0 },
 };

