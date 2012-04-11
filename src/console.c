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

#include "console.h"

extern void console_version ()
{
	g_print ("qinfo " VERSION "\n");
}

extern void console_output (qinfo_t *app, const gchar *file)
{
	if (file == NULL)
		return;

	gchar *text = NULL, *utf8 = NULL;
	if (g_file_get_contents (file, &text, NULL, &app->error) == TRUE &&
		(utf8 = g_convert (text, -1, "UTF-8", "437", NULL, NULL, &app->error))
		!=  NULL)
	{
		g_print (utf8);
		g_print ("\n");
	} else {
		g_warning (app->error->message);
		FREE_GERROR (app->error);
	}

	g_free (text);
	g_free (utf8);
}
