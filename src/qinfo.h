/**
 * qinfo - GTK+ based ascii art text viewer
 * Copyright (C) 2008 Oliver Mader <dotb52@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef QINFO_H
#define QINFO_H

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#define FREE_GERROR(error) if ((error) != NULL) \
	g_error_free(error); \
	(error) = NULL

typedef struct qinfo {
	GtkBuilder *builder;
	GError *error;
	GtkWidget *window;
	GtkWidget *view;
	GtkTextBuffer *buffer;
	GtkTextTag *tag;
	PangoFontDescription *font;
	GdkColor fg;
	GdkColor bg;
	gchar *link;
	gchar *browser;
	gboolean hovering;
	GdkCursor *cursor;
	GdkCursor *hand;
	int x, y;
	int width, height;
	GdkPixbuf *icon;
} qinfo_t;

extern void qinfo_error (GtkWidget *parent, GError **error);

#endif
