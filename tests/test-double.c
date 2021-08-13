/*
 * SPDX-FileCopyrightText: 2021 Red Hat Inc.
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Bastien Nocera <hadess@hadess.net>
 */

#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <glib.h>
#include <gio/gio.h>
#include <umockdev.h>
#include <langinfo.h>

#include <gudev/gudev.h>

static void
test_double (void)
{
	/* create test bed */
	UMockdevTestbed *testbed = umockdev_testbed_new ();

	/* Relies on a test bed having been set up */
	g_assert (umockdev_in_mock_environment ());

	g_assert_cmpstr (nl_langinfo(RADIXCHAR), ==, ",");

	umockdev_testbed_add_device (testbed, "platform", "dev1", NULL,
				     "in_accel_scale", "0.0000098", NULL,
				     "ID_MODEL", "KoolGadget", "SCALE", "0.0000098", NULL);

	/* Check the number of items in GUdevClient */
	const gchar *subsystems[] = { "platform", NULL};
	GUdevClient *client = g_udev_client_new (subsystems);
	GUdevDevice *dev;
	g_autofree char *path = NULL;
	gdouble scale;

	GList *devices = g_udev_client_query_by_subsystem (client, NULL);
	g_assert_cmpint (g_list_length (devices), ==, 1);
	dev = devices->data;

	scale = g_udev_device_get_sysfs_attr_as_double (dev, "in_accel_scale");
	g_assert_cmpfloat (scale, ==, 0.0000098);

	scale = g_udev_device_get_property_as_double (dev, "SCALE");
	g_assert_cmpfloat (scale, ==, 0.0000098);

	g_list_free_full (devices, g_object_unref);
}

int main(int argc, char **argv)
{
	setlocale (LC_ALL, NULL);
	setlocale (LC_NUMERIC, "fr_FR.UTF-8");
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/gudev/double", test_double);

	return g_test_run ();
}
