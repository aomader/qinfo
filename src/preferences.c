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

#include "preferences.h"

static void preferences_read (qinfo_t *app)
{
	GtkFontButton *font = GTK_FONT_BUTTON (gtk_builder_get_object (app->builder,
		"font"));
	GtkColorButton *text = GTK_COLOR_BUTTON (gtk_builder_get_object (
		app->builder, "text"));
	GtkColorButton *background = GTK_COLOR_BUTTON (gtk_builder_get_object (
		app->builder, "background"));
	GtkColorButton *link = GTK_COLOR_BUTTON (gtk_builder_get_object (
		app->builder, "link"));
	GtkComboBox *wrap = GTK_COMBO_BOX (gtk_builder_get_object (app->builder,
		"wordwrap"));
	GtkEntry *browser = GTK_ENTRY (gtk_builder_get_object (app->builder,
		"action"));
	GtkSpinButton *left = GTK_SPIN_BUTTON (gtk_builder_get_object (app->builder,
		"left"));
	GtkSpinButton *right = GTK_SPIN_BUTTON (gtk_builder_get_object (
		app->builder, "right"));
	GtkSpinButton *above = GTK_SPIN_BUTTON (gtk_builder_get_object (
		app->builder, "above"));
	GtkSpinButton *below = GTK_SPIN_BUTTON (gtk_builder_get_object (
		app->builder, "below"));

	gchar *str = pango_font_description_to_string (app->font);
	gtk_font_button_set_font_name (font, str);
	g_free (str);

	gtk_color_button_set_color (text, &app->fg);
	gtk_color_button_set_color (background, &app->bg);

	gtk_combo_box_set_active (wrap, gtk_text_view_get_wrap_mode (GTK_TEXT_VIEW (
		app->view)));

	GdkColor color;
	gdk_color_parse (app->link, &color);
	gtk_color_button_set_color (link, &color);

	gtk_entry_set_text (browser, app->browser);

	gtk_spin_button_set_value (left, (gdouble) gtk_text_view_get_left_margin (
		GTK_TEXT_VIEW (app->view)));
	gtk_spin_button_set_value (right, (gdouble) gtk_text_view_get_right_margin (
		GTK_TEXT_VIEW (app->view)));
	gtk_spin_button_set_value (above, (gdouble)
		gtk_text_view_get_pixels_above_lines (GTK_TEXT_VIEW (app->view)));
	gtk_spin_button_set_value (below, (gdouble)
		gtk_text_view_get_pixels_below_lines (GTK_TEXT_VIEW (app->view)));
}

static void preferences_write (qinfo_t *app)
{
	GtkFontButton *font = GTK_FONT_BUTTON (gtk_builder_get_object (app->builder,
		"font"));
	GtkColorButton *text = GTK_COLOR_BUTTON (gtk_builder_get_object (
		app->builder, "text"));
	GtkColorButton *background = GTK_COLOR_BUTTON (gtk_builder_get_object (
		app->builder, "background"));
	GtkColorButton *link = GTK_COLOR_BUTTON (gtk_builder_get_object (
		app->builder, "link"));
	GtkComboBox *wrap = GTK_COMBO_BOX (gtk_builder_get_object (app->builder,
		"wordwrap"));
	GtkEntry *browser = GTK_ENTRY (gtk_builder_get_object (app->builder,
		"action"));
	GtkSpinButton *left = GTK_SPIN_BUTTON (gtk_builder_get_object (app->builder,
		"left"));
	GtkSpinButton *right = GTK_SPIN_BUTTON (gtk_builder_get_object (
		app->builder, "right"));
	GtkSpinButton *above = GTK_SPIN_BUTTON (gtk_builder_get_object (
		app->builder, "above"));
	GtkSpinButton *below = GTK_SPIN_BUTTON (gtk_builder_get_object (
		app->builder, "below"));

	GKeyFile *cfg = g_key_file_new ();

	gchar *file = g_build_filename (g_get_user_config_dir (), "qinfo", NULL);

	if (g_key_file_load_from_file (cfg, file, G_KEY_FILE_NONE, &app->error) ==
		FALSE)
	{
		g_warning (app->error->message);
	}

	FREE_GERROR(app->error);

	const gchar *fontname = gtk_font_button_get_font_name (font);
	pango_font_description_free (app->font);
	app->font = pango_font_description_from_string (fontname);
	gtk_widget_modify_font (app->view, app->font);
	g_key_file_set_string (cfg, "textview", "font", fontname);

	gtk_color_button_get_color (text, &app->fg);
	gtk_widget_modify_text (app->view, GTK_STATE_NORMAL, &app->fg);
	gchar *str = gdk_color_to_string (&app->fg);
	g_key_file_set_string (cfg, "textview", "text_color", str);
	g_free (str);

	gtk_color_button_get_color (background, &app->bg);
	gtk_widget_modify_base (app->view, GTK_STATE_NORMAL, &app->bg);
	str = gdk_color_to_string (&app->bg);
	g_key_file_set_string (cfg, "textview", "background_color", str);
	g_free (str);

	gint value = gtk_combo_box_get_active (wrap);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (app->view), value);
	g_key_file_set_integer (cfg, "textview", "word_wrap", value);

	GdkColor color;
	gtk_color_button_get_color (link, &color);
	str = gdk_color_to_string (&color);
	g_free (app->link);
	app->link = str;
	g_object_set (app->tag, "foreground", app->link, NULL);
	g_key_file_set_string (cfg, "textview", "link_color", str);

	const char *action = gtk_entry_get_text (browser);
	g_free (app->browser);
	app->browser = g_strdup (action);
	g_key_file_set_string (cfg, "textview", "browser", action);

	value = gtk_spin_button_get_value_as_int (left);
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW (app->view), value);
	g_key_file_set_integer (cfg, "textview", "left_margin", value);

	value = gtk_spin_button_get_value_as_int (right);
	gtk_text_view_set_right_margin (GTK_TEXT_VIEW (app->view), value);
	g_key_file_set_integer (cfg, "textview", "right_margin", value);

	value = gtk_spin_button_get_value_as_int (above);
	gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (app->view), value);
	g_key_file_set_integer (cfg, "textview", "pixels_above_lines", value);

	value = gtk_spin_button_get_value_as_int (below);
	gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (app->view), value);
	g_key_file_set_integer (cfg, "textview", "pixels_below_lines", value);

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

extern void preferences_init (qinfo_t *app)
{
	if (gtk_builder_add_from_file (app->builder, UI_DIR "/preferences.xml",
		&app->error) == 0)
	{
		qinfo_error (app->window, &app->error);
		return;
	}

	GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (app->builder,
		"preferences_dialog"));
	gtk_window_set_icon (GTK_WINDOW (dialog), app->icon);

	preferences_read (app);

	gtk_builder_connect_signals (app->builder, app);
	gtk_widget_show (dialog);
}

extern void preferences_finish (qinfo_t *app, GtkDialog *dialog, gint response)
{
	if (response == 0)
		return;

	if (response == GTK_RESPONSE_OK)
		preferences_write (app);

	gtk_widget_destroy (GTK_WIDGET (dialog));
}
