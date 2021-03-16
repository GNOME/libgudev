/* umockdev example: use libumockdev in C to fake a battery
 * Build with:
 * gcc battery.c -Wall `pkg-config --cflags --libs umockdev-1.0 gio-2.0` -o /tmp/battery
 * Run with:
 * umockdev-wrapper /tmp/battery
 *
 * Copyright (C) 2013 Canonical Ltd.
 * Author: Martin Pitt <martin.pitt@ubuntu.com>
 *
 * umockdev is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * umockdev is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <glib.h>
#include <gio/gio.h>
#include <umockdev.h>

#include <gudev/gudev.h>

static void
test_uncached_sysfs_attr (void)
{
	/* create test bed */
	UMockdevTestbed *testbed = umockdev_testbed_new ();

	/* Relies on a test bed having been set up */
	g_assert (umockdev_in_mock_environment ());

	umockdev_testbed_add_device (testbed, "platform", "dev1", NULL,
				     "dytc_lapmode", "1", "console", "Y\n", NULL,
				     "ID_MODEL", "KoolGadget", NULL);

	/* Check the number of items in GUdevClient */
	const gchar *subsystems[] = { "platform", NULL};
	GUdevClient *client = g_udev_client_new (subsystems);
	GUdevDevice *dev;
	g_autofree char *lapmode_path = NULL;
	g_autofree char *console_path = NULL;
	FILE *sysfsfp;

	GList *devices = g_udev_client_query_by_subsystem (client, NULL);
	g_assert_cmpint (g_list_length (devices), ==, 1);
	dev = devices->data;
	lapmode_path = g_build_filename (g_udev_device_get_sysfs_path (dev), "dytc_lapmode", NULL);
	/* First access */
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));
	sysfsfp = fopen (lapmode_path, "w");
	fprintf (sysfsfp, "%s\n", "0");
	fclose (sysfsfp);
	/* This is cached */
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));
	/* This is uncached, and updates the cache */
	g_assert_false (g_udev_device_get_sysfs_attr_as_boolean_uncached (dev, "dytc_lapmode"));
	g_assert_false (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));

	/* Test N/Y and trailing linefeeds */
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean (dev, "console"));
	console_path = g_build_filename (g_udev_device_get_sysfs_path (dev), "console", NULL);
	sysfsfp = fopen (console_path, "w");
	fprintf (sysfsfp, "%s\n", "N");
	fclose (sysfsfp);
	g_assert_false (g_udev_device_get_sysfs_attr_as_boolean_uncached (dev, "console"));
	sysfsfp = fopen (console_path, "w");
	fprintf (sysfsfp, "%s\n", "Y");
	fclose (sysfsfp);
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean_uncached (dev, "console"));

	g_list_free_full (devices, g_object_unref);
}

int main(int argc, char **argv)
{
	setlocale (LC_ALL, NULL);
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/gudev/uncached_sysfs_attr", test_uncached_sysfs_attr);

	return g_test_run ();
}
