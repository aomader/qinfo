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

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "qinfo.h"
#include "hyperlink.h"
#include "open.h"
#include "preferences.h"
#include "about.h"

G_MODULE_EXPORT void on_window_destroy (GtkObject *object, qinfo_t *app);
G_MODULE_EXPORT gboolean on_window_configure_event (GtkWidget *widget,
	GdkEventConfigure *event, qinfo_t *app);
G_MODULE_EXPORT void on_open_toolbar_item_clicked (GtkToolButton *button,
	qinfo_t *app);
G_MODULE_EXPORT void on_open_toolbar_item_activated (GtkRecentChooser *chooser,
	qinfo_t *app);
G_MODULE_EXPORT void on_all_toolbar_item_clicked (GtkToolButton *button,
	qinfo_t *app);
G_MODULE_EXPORT void on_copy_toolbar_item_clicked (GtkToolButton *button,
	qinfo_t *app);
G_MODULE_EXPORT void on_preferences_toolbar_item_clicked (GtkObject *object,
	qinfo_t *app);
G_MODULE_EXPORT void on_preferences_dialog_response (GtkDialog *dialog,
	gint response, qinfo_t *app);
G_MODULE_EXPORT void on_about_toolbar_item_clicked (GtkToolButton *button,
	qinfo_t *app);
G_MODULE_EXPORT gboolean on_textview_key_press_event (GtkWidget *textview,
	GdkEventKey *event, qinfo_t *app);
G_MODULE_EXPORT void on_textview_event_after (GtkWidget *textview,
	GdkEvent *event, qinfo_t *app);
G_MODULE_EXPORT gboolean on_textview_motion_notify_event (GtkWidget *textview,
	GdkEventMotion *event, qinfo_t *app);
G_MODULE_EXPORT gboolean on_textview_visibility_notify_event (
	GtkWidget *textview, GdkEventVisibility *event, qinfo_t *app);
G_MODULE_EXPORT void on_textview_drag_data_received (GtkWidget *widget,
	GdkDragContext *context, gint x, gint y, GtkSelectionData *selection_data,
	guint target_type, guint time, qinfo_t *app);
G_MODULE_EXPORT gboolean on_textview_drag_motion (GtkWidget *widget,
	GdkDragContext *drag_context, gint x, gint y, guint time, qinfo_t *app);

#endif
