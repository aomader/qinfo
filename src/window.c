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

#include "window.h"

static void window_recent (qinfo_t *app)
{
	GtkRecentFilter *filter = gtk_recent_filter_new ();
	gtk_recent_filter_add_application (filter, "qinfo");

	GtkWidget *recent = gtk_recent_chooser_menu_new ();
	gtk_recent_chooser_set_sort_type (GTK_RECENT_CHOOSER (recent),
		GTK_RECENT_SORT_MRU);
	gtk_recent_chooser_set_limit (GTK_RECENT_CHOOSER (recent), 10);
	gtk_recent_chooser_add_filter (GTK_RECENT_CHOOSER (recent), filter);

	GtkWidget *button = GTK_WIDGET (gtk_builder_get_object (app->builder,
		"open_toolbar_item"));
	gtk_menu_tool_button_set_menu (GTK_MENU_TOOL_BUTTON (button), recent);

	g_signal_connect (GTK_RECENT_CHOOSER (recent), "item-activated",
		G_CALLBACK (on_open_toolbar_item_activated), app);
}

static void window_dnd (qinfo_t *app)
{
	GtkTargetEntry types[] = {
		{"text/uri-list", 0, 0}
	};

	gtk_drag_dest_set (app->view, GTK_DEST_DEFAULT_ALL, types, 1,
		GDK_ACTION_COPY);
}

extern void window_init (qinfo_t *app, const gchar *file)
{
	app->builder = gtk_builder_new ();

	if (gtk_builder_add_from_file (app->builder, UI_DIR "/main.xml",
		&app->error) == 0)
	{
		qinfo_error (NULL, &app->error);
		return;
	}

	app->window = GTK_WIDGET (gtk_builder_get_object (app->builder, "window"));
	app->view = GTK_WIDGET (gtk_builder_get_object (app->builder, "textview"));
	app->buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (app->view));
	app->tag = gtk_text_buffer_create_tag (app->buffer, NULL, "foreground",
		"blue", "underline", PANGO_UNDERLINE_SINGLE, NULL);
	app->cursor = gdk_cursor_new (GDK_XTERM);
	app->hand = gdk_cursor_new (GDK_HAND2);
	app->icon = gdk_pixbuf_new_from_file (PIXMAPS_DIR "/qinfo.png", NULL);

	gtk_window_set_icon (GTK_WINDOW (app->window), app->icon);

	gtk_builder_connect_signals (app->builder, app);

	window_recent (app);
	window_dnd (app);

	settings_read (app);

	gtk_widget_show (app->window);

	if (file != NULL)
		open_file (app, file);

	gtk_main ();

	settings_write (app);

	gdk_colormap_free_colors (gdk_colormap_get_system (), &app->fg, 1);
	gdk_colormap_free_colors (gdk_colormap_get_system (), &app->bg, 1);
	pango_font_description_free (app->font);
	g_free (app->browser);
	g_free (app->link);
	gdk_cursor_unref (app->cursor);
	gdk_cursor_unref (app->hand);
	g_object_unref (app->icon);
	g_object_unref (app->builder);
}
