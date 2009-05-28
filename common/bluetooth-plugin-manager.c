/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2009  Bastien Nocera <hadess@hadess.net>
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <bluetooth-plugin.h>
#include "bluetooth-plugin-manager.h"

#define UNINSTALLED_PLUGINDIR "../common/plugins"

GList *plugin_list = NULL;

static void
bluetooth_plugin_dir_process (const char *plugindir)
{
	GDir *dir;
	const char *item;
	GbtPlugin *p = NULL;
	GError *err = NULL;

	dir = g_dir_open (plugindir, 0, &err);

	if (dir == NULL) {
		g_warning ("Can't open the plugins dir: %s", err ? err->message : "No reason");
		if (err)
			g_error_free (err);
	} else {
		while ((item = g_dir_read_name(dir))) {
			if (g_str_has_suffix (item, SOEXT)) {
				char *module_path;

				p = g_new0(GbtPlugin, 1); 
				module_path = g_module_build_path (plugindir, item);
				p->module = g_module_open (module_path, G_MODULE_BIND_LAZY);
				if (!p->module) {
					g_warning ("error opening %s: %s", module_path, g_module_error ());
					g_free (module_path);
					continue;
				}
				g_free (module_path);

				plugin_list = g_list_append (plugin_list, p); 
			}
		}
		g_dir_close (dir);
	}
}

gboolean
bluetooth_plugin_manager_init (void)
{
	if (g_file_test (UNINSTALLED_PLUGINDIR, G_FILE_TEST_IS_DIR) != FALSE) {
		/* Try to load the local plugins */
		bluetooth_plugin_dir_process ("../common/plugins/.libs/");
	}

	if (g_list_length (plugin_list) == 0)
		bluetooth_plugin_dir_process (PLUGINDIR);

	return g_list_length (plugin_list) != 0;
}

void
bluetooth_plugin_manager_cleanup (void)
{
	GList *l;

	for (l = plugin_list; l != NULL; l = l->next) {
		GbtPlugin *p = l->data;

		g_module_close (p->module);
		g_free (p);
	}
	g_list_free (plugin_list);
	plugin_list = NULL;
}

