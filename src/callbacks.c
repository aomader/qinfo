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

#include "callbacks.h"

G_MODULE_EXPORT void on_window_destroy (GtkObject *object, qinfo_t *app)
{
	gtk_main_quit();
}

G_MODULE_EXPORT gboolean on_window_configure_event (GtkWidget *widget,
	GdkEventConfigure *event, qinfo_t *app)
{
	app->x = event->x;
	app->y = event->y;
	app->width = event->width;
	app->height = event->height;

	return FALSE;
}

G_MODULE_EXPORT void on_open_toolbar_item_clicked (GtkToolButton *button,
	qinfo_t *app)
{
	open_init (app);
}

G_MODULE_EXPORT void on_open_toolbar_item_activated (GtkRecentChooser *chooser,
	qinfo_t *app)
{
	gchar *file, *uri = gtk_recent_chooser_get_current_uri (chooser);

	if ((file = g_filename_from_uri (uri, NULL, &app->error)) != NULL) {
		open_file (app, file);
		g_free (file);
	}

	g_free (uri);
}

G_MODULE_EXPORT void on_all_toolbar_item_clicked (GtkToolButton *button,
	qinfo_t *app)
{
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds (app->buffer, &start, &end);
	gtk_text_buffer_select_range (app->buffer, &start, &end);
}

G_MODULE_EXPORT void on_copy_toolbar_item_clicked (GtkToolButton *button,
	qinfo_t *app)
{
	GtkClipboard *clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
	gtk_text_buffer_copy_clipboard (app->buffer, clipboard);
}

G_MODULE_EXPORT void on_preferences_toolbar_item_clicked (GtkObject *object,
	qinfo_t *app)
{
	preferences_init (app);
}

G_MODULE_EXPORT void on_preferences_dialog_response (GtkDialog *dialog,
	gint response, qinfo_t *app)
{
	preferences_finish (app, dialog, response);
}

G_MODULE_EXPORT void on_about_toolbar_item_clicked (GtkToolButton *button,
	qinfo_t *app)
{
	about_init (app);
}

G_MODULE_EXPORT gboolean on_textview_key_press_event (GtkWidget *textview,
	GdkEventKey *event, qinfo_t *app)
{
	if (event->keyval == GDK_Return || event->keyval == GDK_KP_Enter) {
		GtkTextIter iter;
		gtk_text_buffer_get_iter_at_mark (app->buffer, &iter,
			gtk_text_buffer_get_insert (app->buffer));

		hyperlink_follow (app, &iter);
	}

	return FALSE;
}

G_MODULE_EXPORT void on_textview_event_after (GtkWidget *textview,
	GdkEvent *event, qinfo_t *app)
{
	if (event->type != GDK_BUTTON_RELEASE)
		return;

	GdkEventButton *event_btn = (GdkEventButton *) event;

	if (event_btn->button != 1)
		return;

	GtkTextIter start, end;
	gtk_text_buffer_get_selection_bounds (app->buffer, &start, &end);

	if (gtk_text_iter_get_offset (&start) != gtk_text_iter_get_offset (&end))
		return;

	gint x, y;
	gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (textview),
		GTK_TEXT_WINDOW_WIDGET, event_btn->x, event_btn->y, &x, &y);

	GtkTextIter iter;
	gtk_text_view_get_iter_at_location (GTK_TEXT_VIEW (textview), &iter, x, y);

	hyperlink_follow (app, &iter);
}

G_MODULE_EXPORT gboolean on_textview_motion_notify_event (GtkWidget *textview,
	GdkEventMotion *event, qinfo_t *app)
{
	gint x, y;
	gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (textview),
		GTK_TEXT_WINDOW_WIDGET, event->x, event->y, &x, &y);

	hyperlink_cursor(app, x, y);

	return FALSE;
}

G_MODULE_EXPORT gboolean on_textview_visibility_notify_event (
	GtkWidget *textview, GdkEventVisibility *event, qinfo_t *app)
{
	gint wx, wy;
	gdk_window_get_pointer (textview->window, &wx, &wy, NULL);

	gint bx, by;
	gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (textview),
		GTK_TEXT_WINDOW_WIDGET, wx, wy, &bx, &by);

	hyperlink_cursor (app, bx, by);

	return FALSE;
}

G_MODULE_EXPORT void on_textview_drag_data_received (GtkWidget *widget,
	GdkDragContext *context, gint x, gint y, GtkSelectionData *selection_data,
	guint target_type, guint time, qinfo_t *app)
{
	if (selection_data->data != NULL && g_strstr_len ((gchar *)
		selection_data->data, 5, "file:"))
	{
		gchar **files = g_strsplit_set ((gchar *) selection_data->data, "\r\n",
			2);
		gchar *file = g_filename_from_uri (files[0], NULL, NULL);

		if (file != NULL)
			open_file (app, file);

		g_free (file);
		g_strfreev (files);
	}
}

G_MODULE_EXPORT gboolean on_textview_drag_motion (GtkWidget *widget,
	GdkDragContext *drag_context, gint x, gint y, guint time, qinfo_t *app)
{
	return TRUE;
}
/*
G_MODULE_EXPORT void on_textview_size_request (GtkWidget *widget,
	GtkRequisition *requisition, qinfo_t *app)
{
	if (app->expand == FALSE)
		return;

	GdkRectangle rect;
	gdk_window_get_frame_extents (GTK_WIDGET (app->window)->window, &rect);

	gint border = (rect.width - app->width) / 2;
	gint width = requisition->width;

	if (GTK_WIDGET (app->view)->allocation.height < requisition->height) {
		g_print ("added\n");
		width += 22;
	}

	if (app->width < width) {
		gint screen = gdk_screen_get_width (gtk_window_get_screen (GTK_WINDOW (
			app->window))) - border * 2;

		if (width > screen) {
			gtk_window_move (GTK_WINDOW (app->window), border, app->y);
			width = screen;
		} else if (width > screen - app->x) {
			gtk_window_move (GTK_WINDOW (app->window), screen - width, app->y);
		}

		gtk_window_resize (GTK_WINDOW (app->window), width, app->height);
	}
}
*/
