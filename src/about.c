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

#include "about.h"

extern void about_init (qinfo_t *app)
{
	if (gtk_builder_add_from_file (app->builder, UI_DIR "/about.xml",
		&app->error) == 0)
	{
		qinfo_error (app->window, &app->error);
		return;
	}

	GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (app->builder,
		"about_dialog"));
	gtk_window_set_icon (GTK_WINDOW (dialog), app->icon);

	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), VERSION);
	gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG (dialog), app->icon);

	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}
