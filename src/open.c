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

#include "open.h"

extern void open_init (qinfo_t *app)
{
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open file ...",
		GTK_WINDOW (app->window), GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN,
		GTK_RESPONSE_ACCEPT, NULL);

	gtk_window_set_icon (GTK_WINDOW (dialog), app->icon);

	GtkFileFilter *ascii = gtk_file_filter_new ();
	gtk_file_filter_set_name (ascii, "Ascii art documents");
	gtk_file_filter_add_pattern (ascii, "*.nfo");
	gtk_file_filter_add_pattern (ascii, "*.diz");

	GtkFileFilter *all = gtk_file_filter_new ();
	gtk_file_filter_set_name (all, "All files");
	gtk_file_filter_add_pattern (all, "*");

	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), ascii);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), all);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT ) {
		gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (
			dialog));

		if (filename != NULL) {
			open_file (app, filename);
			g_free (filename);
		}
	}

	gtk_widget_destroy (dialog);
}

extern void open_file (qinfo_t *app, const gchar *file)
{
	gchar *text = NULL, *utf8 = NULL;
	if (g_file_get_contents (file, &text, NULL, &app->error) == FALSE ||
		(utf8 = g_convert (text, -1, "UTF-8", "437", NULL, NULL, &app->error))
		== NULL)
	{
		qinfo_error (app->window, &app->error);
		g_free (text);
		return;
	}

	g_free (text);

	GtkRecentData recent;
	recent.display_name = NULL;
	recent.description = NULL;
	recent.mime_type = "text/plain";
	recent.app_name = "qinfo";
	recent.app_exec = "qinfo %f";
	recent.groups = NULL;
	recent.is_private = FALSE;

	gchar *uri = g_filename_to_uri (file, NULL, NULL);

	if (G_LIKELY (uri != NULL)) {
		gtk_recent_manager_add_full (gtk_recent_manager_get_default (), uri,
			&recent);
		g_free (uri);
	}

	gtk_text_buffer_set_text (app->buffer, "", -1);

	GtkTextIter iter;
	gtk_text_buffer_get_start_iter (app->buffer, &iter);

	gchar *p = utf8, *last = utf8;

	while (*p != 0x00) {
		if (*p == ':' && *(p + 1) == '/' && *(p + 2) == '/') {
			gchar *start = p, *end = p;

			while (g_ascii_isalpha(*(--start)) == TRUE);
			++start;

			while (g_ascii_isalnum (*(++end)) == TRUE || *end == '/' ||
				*end == '%' || *end == '.' || *end == '-' || *end == '_' ||
				*end == '?' || *end == '=');
			--end;

			gtk_text_buffer_insert(app->buffer, &iter, last, start - last);
			gtk_text_buffer_insert_with_tags(app->buffer, &iter, start,
				end - start + 1, app->tag, NULL);

			last = p = end + 1;
		} else if ((*p == 'w' || *p == 'W') && (*(p + 1) == 'w' ||
			*(p + 1) == 'W') && (*(p + 2) == 'w' || *(p + 2) == 'W') &&
			*(p + 3) == '.')
		{
			gchar *end = p + 3;

			while (g_ascii_isalnum (*(++end)) == TRUE || *end == '/' ||
				*end == '%' || *end == '.');
			--end;

			gtk_text_buffer_insert (app->buffer, &iter, last, p - last);
			gtk_text_buffer_insert_with_tags (app->buffer, &iter, p,
				end - p + 1, app->tag, NULL);

			last = p = end + 1;
		} else {
			++p;
		}
	}

	gtk_text_buffer_insert (app->buffer, &iter, last, p - last);

	g_free (utf8);

	gchar *base = g_filename_display_basename (file);
	gchar *title = g_strconcat (base, " - qinfo", NULL);
	gtk_window_set_title(GTK_WINDOW(app->window), title);
	g_free (title);
	g_free (base);
}
