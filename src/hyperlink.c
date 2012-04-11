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

#include "hyperlink.h"

extern void hyperlink_cursor (qinfo_t *app, gint x, gint y)
{
	gboolean hovering = FALSE;

	GtkTextIter iter;
	gtk_text_view_get_iter_at_location (GTK_TEXT_VIEW (app->view), &iter, x, y);

	GSList *tags = NULL, *tagp = NULL;
	tags = gtk_text_iter_get_tags (&iter);

	GtkTextTag *tag;

	for (tagp = tags; tagp != NULL; tagp = tagp->next) {
		tag = tagp->data;

		if (tag == app->tag) {
			hovering = TRUE;
			break;
		}
	}

	if (hovering != app->hovering) {
		app->hovering = hovering;

		if (app->hovering)
			gdk_window_set_cursor (gtk_text_view_get_window (
				GTK_TEXT_VIEW (app->view), GTK_TEXT_WINDOW_TEXT), app->hand);
		else
			gdk_window_set_cursor (gtk_text_view_get_window(
				GTK_TEXT_VIEW (app->view), GTK_TEXT_WINDOW_TEXT), app->cursor);
	}

	g_slist_free (tags);
}

extern void hyperlink_follow (qinfo_t *app, GtkTextIter *iter)
{
	GSList *tags = NULL, *tagp = NULL;
	tags = gtk_text_iter_get_tags (iter);

	for (tagp = tags;  tagp != NULL;  tagp = tagp->next) {
		GtkTextTag *tag = tagp->data;

		if (tag == app->tag) {
			GtkTextIter *start = gtk_text_iter_copy (iter), *end =
				gtk_text_iter_copy (iter);

			if (gtk_text_iter_begins_tag (start, app->tag) == FALSE)
				gtk_text_iter_backward_to_tag_toggle (start, app->tag);

			if (gtk_text_iter_ends_tag (end, app->tag) == FALSE)
				gtk_text_iter_forward_to_tag_toggle (end, app->tag);

			gchar *link = gtk_text_buffer_get_text (app->buffer, start, end,
				FALSE);
			gchar *cmd = g_strdup_printf (app->browser, link);

			if (g_spawn_command_line_async (cmd, &app->error) == FALSE)
				qinfo_error (app->window, &app->error);

			g_free (cmd);
			g_free (link);

			gtk_text_iter_free (start);
			gtk_text_iter_free (end);

			break;
		}
	}

	g_slist_free (tags);
}
