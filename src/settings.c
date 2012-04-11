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

#include "settings.h"

extern void settings_read (qinfo_t *app)
{
	GKeyFile *cfg = g_key_file_new ();

	gchar *file = g_build_filename (g_get_user_config_dir (), "qinfo", NULL);

	if (g_key_file_load_from_file (cfg, file, G_KEY_FILE_NONE, &app->error) ==
		FALSE)
	{
		g_warning (app->error->message);
	}

	g_free (file);

	FREE_GERROR (app->error);

	app->x = g_key_file_get_integer (cfg, "window", "x", &app->error);
	if (app->error != NULL)
		app->x = 100;

	FREE_GERROR (app->error);

	app->y = g_key_file_get_integer (cfg, "window", "y", &app->error);
	if (app->error != NULL)
		app->y = 100;

	FREE_GERROR (app->error);

	app->width = g_key_file_get_integer (cfg, "window", "width", &app->error);
	if (app->error != NULL)
		app->width = 500;

	FREE_GERROR (app->error);

	app->height = g_key_file_get_integer (cfg, "window", "height", &app->error);
	if (app->error != NULL)
		app->height = 600;

	FREE_GERROR (app->error);

	gtk_window_resize (GTK_WINDOW (app->window), app->width, app->height);
	gtk_window_move (GTK_WINDOW (app->window), app->x, app->y);

	gchar *str = g_key_file_get_string (cfg, "textview", "font", &app->error);
	if (app->error != NULL)
		str = g_strdup ("Terminus 8");
	app->font = pango_font_description_from_string (str);

	FREE_GERROR (app->error);

	str = g_key_file_get_string (cfg, "textview", "text_color", &app->error);
	if (app->error != NULL)
		str = g_strdup ("#000000");
	gdk_color_parse (str, &app->fg);
	gdk_colormap_alloc_color (gdk_colormap_get_system (), &app->fg, FALSE,
		TRUE);
	g_free (str);

	FREE_GERROR (app->error);

	str = g_key_file_get_string (cfg, "textview", "background_color",
		&app->error);
	if (app->error != NULL)
		str = g_strdup ("#FFFFFF");
	gdk_color_parse (str, &app->bg);
	gdk_colormap_alloc_color (gdk_colormap_get_system (), &app->bg, FALSE,
		TRUE);
	g_free (str);

	FREE_GERROR (app->error);

	str = g_key_file_get_string (cfg, "textview", "link_color", &app->error);
	if (app->error != NULL)
		app->link = g_strdup ("#0000ff");
	else
		app->link = str;

	FREE_GERROR(app->error);

	str = g_key_file_get_string (cfg, "textview", "browser", &app->error);
	if (app->error == NULL)
		app->browser = str;
	else
		app->browser = g_strdup ("xdg-open \"%s\"");

	FREE_GERROR(app->error);

	gtk_widget_modify_font (app->view, app->font);
	gtk_widget_modify_text (app->view, GTK_STATE_NORMAL, &app->fg);
	gtk_widget_modify_base (app->view, GTK_STATE_NORMAL, &app->bg);
	g_object_set (app->tag, "foreground", app->link, NULL);

	gint value = g_key_file_get_integer (cfg, "textview", "word_wrap",
		&app->error);
	if (app->error == NULL)
		gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (app->view), value);

	FREE_GERROR (app->error);

	value = g_key_file_get_integer (cfg, "textview", "left_margin",
		&app->error);
	if (app->error == NULL)
		gtk_text_view_set_left_margin (GTK_TEXT_VIEW (app->view), value);

	FREE_GERROR (app->error);

	value = g_key_file_get_integer (cfg, "textview", "right_margin",
		&app->error);
	if (app->error == NULL)
		gtk_text_view_set_right_margin (GTK_TEXT_VIEW (app->view), value);

	FREE_GERROR (app->error);

	value = g_key_file_get_integer (cfg, "textview", "pixels_above_lines",
		&app->error);
	if (app->error == NULL)
		gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (app->view), value);

	FREE_GERROR (app->error);

	value = g_key_file_get_integer (cfg, "textview", "pixels_below_lines",
		&app->error);
	if (app->error == NULL)
		gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (app->view), value);

	FREE_GERROR (app->error);

	g_key_file_free (cfg);
}

extern void settings_write (qinfo_t *app)
{
	gchar *file = g_build_filename (g_get_user_config_dir (), "qinfo", NULL);
	GKeyFile *cfg = g_key_file_new ();

	if (g_key_file_load_from_file (cfg, file, G_KEY_FILE_NONE, &app->error) ==
		FALSE)
	{
		g_warning (app->error->message);
	}

	FREE_GERROR (app->error);

	g_key_file_set_integer (cfg, "window", "width", app->width);
	g_key_file_set_integer (cfg, "window", "height", app->height);
	g_key_file_set_integer (cfg, "window", "x", app->x);
	g_key_file_set_integer (cfg, "window", "y", app->y);

	gsize length;
	gchar *data = g_key_file_to_data (cfg, &length, &app->error);

	g_key_file_free (cfg);

	if (app->error != NULL) {
		g_warning (app->error->message);
	} else {
		if (g_file_set_contents (file, data, length, &app->error) == FALSE)
			g_warning (app->error->message);

		g_free (data);
	}

	g_free (file);

	FREE_GERROR (app->error);
}
