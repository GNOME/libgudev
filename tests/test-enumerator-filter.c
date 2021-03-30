/* umockdev example: use libumockdev in C to fake a battery
 * Build with:
 * gcc battery.c -Wall `pkg-config --cflags --libs umockdev-1.0 gio-2.0` -o /tmp/battery
 * Run with:
 * umockdev-wrapper /tmp/battery
 *
 * SPDX-FileCopyrightText: 2013 Canonical Ltd.
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Martin Pitt <martin.pitt@ubuntu.com>
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
test_enumerator_filter (void)
{
	/* create test bed */
	UMockdevTestbed *testbed = umockdev_testbed_new ();

	/* Relies on a test bed having been set up */
	g_assert (umockdev_in_mock_environment ());

	/* Add 2 devices in the USB subsystem, and one in the DRM subsystem */
	umockdev_testbed_add_device (testbed, "usb", "dev1", NULL,
				     "idVendor", "0815", "idProduct", "AFFE", NULL,
				     "ID_MODEL", "KoolGadget", NULL);

	umockdev_testbed_add_device (testbed, "usb", "dev2", NULL,
				     "idVendor", "0815", "idProduct", "AFFF", NULL,
				     "ID_MODEL", "KoolGadget 2", NULL);

	umockdev_testbed_add_device (testbed, "drm", "dev3", NULL,
				     "ID_FOR_SEAT", "drm-pci-0000_00_02_0", NULL,
				     NULL);

	/* Check the number of items in GUdevClient */
	const gchar *subsystems[] = { "drm", NULL};
	GUdevClient *client = g_udev_client_new (subsystems);

	GList *devices = g_udev_client_query_by_subsystem (client, NULL);
	g_assert_cmpint (g_list_length (devices), ==, 3);
	g_list_free_full (devices, g_object_unref);

	devices = g_udev_client_query_by_subsystem (client, "usb");
	g_assert_cmpint (g_list_length (devices), ==, 2);
	g_list_free_full (devices, g_object_unref);

	/* Check the number of items in GUdevEnumerator */
	GUdevEnumerator *enumerator = g_udev_enumerator_new (client);
	devices = g_udev_enumerator_execute (enumerator);
	g_assert_cmpint (g_list_length (devices), ==, 1);
	g_list_free_full (devices, g_object_unref);
}

int main(int argc, char **argv)
{
	setlocale (LC_ALL, NULL);
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/gudev/enumerator_filter", test_enumerator_filter);

	return g_test_run ();
}
