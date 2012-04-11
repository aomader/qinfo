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

#include "qinfo.h"
#include "console.h"
#include "window.h"

int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	qinfo_t *app = g_slice_new (qinfo_t);

	gboolean console = FALSE, version = FALSE;
	gchar **files = NULL, *file = NULL;

	GOptionEntry entries[] = {
		{"console", 'c', 0, G_OPTION_ARG_NONE, &console, "Print the file "
			"to the console", NULL},
		{"version", 'v', 0, G_OPTION_ARG_NONE, &version, "Version "
			"information", NULL},
		{G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &files, "",
			NULL},
		{NULL}
	};

	GOptionContext *context = g_option_context_new ("[FILE]");
	g_option_context_add_main_entries (context, entries, NULL);
	g_option_context_add_group (context, gtk_get_option_group (TRUE));
	g_option_context_set_help_enabled (context, TRUE);
	g_option_context_set_summary (context, "A tiny but strong ascii art text "
		"viewer written in C using GTK+.");

	if (!g_option_context_parse (context, &argc, &argv, &app->error)) {
		g_warning (app->error->message);
		FREE_GERROR (app->error);
	}

	g_option_context_free (context);

	if (files != NULL) {
		file = g_strdup (files[0]);
		g_strfreev (files);
	}

	if (version == TRUE) {
		console_version ();
	} else if (console == TRUE) {
		console_output (app, file);
	} else {
		window_init (app, file);
	}

	g_free (file);
	g_slice_free (qinfo_t, app);

	return 0;
}

extern void qinfo_error (GtkWidget *parent, GError **error)
{
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (parent),
		GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR,
		GTK_BUTTONS_OK, (*error)->message);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);

	g_error_free (*error);
	*error = NULL;
}
