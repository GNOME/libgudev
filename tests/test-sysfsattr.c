/*
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

typedef struct {
	UMockdevTestbed *testbed;
} Fixture;

static void
fixture_setup (Fixture *f, G_GNUC_UNUSED const void *data)
{
	f->testbed = umockdev_testbed_new ();

	g_assert (umockdev_in_mock_environment ());
}

static GUdevDevice*
create_single_dev (Fixture *f, const char *device)
{
	g_autoptr(GError) error = NULL;
	g_autoptr(GUdevClient) client = NULL;
	g_autolist(GUdevDevice) devices;
	GUdevDevice *dev;

	if (!umockdev_testbed_add_from_string (f->testbed, device, &error))
		g_error ("Failed to add test device: %s", error->message);

	client = g_udev_client_new (NULL);

	devices = g_udev_client_query_by_subsystem (client, NULL);
	g_assert_cmpint (g_list_length (devices), ==, 1);

	dev = devices->data;
	devices = g_list_delete_link (devices, devices);

	return dev;
}

static void
fixture_teardown (Fixture *f, G_GNUC_UNUSED const void *data)
{
	g_clear_object (&f->testbed);
}

static void
test_uncached_sysfs_attr (Fixture *f, G_GNUC_UNUSED const void *data)
{
	g_autoptr(GUdevDevice) dev = NULL;
	g_autofree char *lapmode_path = NULL;
	g_autofree char *console_path = NULL;
	FILE *sysfsfp;

	dev = create_single_dev (f, "P: /devices/dev1\n"
	                            "E: SUBSYSTEM=platform\n"
	                            "A: dytc_lapmode=1\n"
	                            "A: console=Y\\n\n"
	                            "E: ID_MODEL=KoolGadget");

	lapmode_path = g_build_filename (g_udev_device_get_sysfs_path (dev), "dytc_lapmode", NULL);
	/* First access */
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));
	g_assert_cmpstr (g_udev_device_get_sysfs_attr (dev, "dytc_lapmode"), ==, "1");
	sysfsfp = fopen (lapmode_path, "w");
	fprintf (sysfsfp, "%s\n", "0");
	fclose (sysfsfp);
	/* This is cached */
	g_assert_true (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));
	/* This is uncached, and updates the cache */
	g_assert_false (g_udev_device_get_sysfs_attr_as_boolean_uncached (dev, "dytc_lapmode"));
	g_assert_false (g_udev_device_get_sysfs_attr_as_boolean (dev, "dytc_lapmode"));
	g_assert_cmpstr (g_udev_device_get_sysfs_attr (dev, "dytc_lapmode"), ==, "0");

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
}

static void
test_sysfs_attr_keys (Fixture *f, G_GNUC_UNUSED const void *data)
{
	const char *expected[] = { "console", "dytc_lapmode", "subsystem", "uevent", NULL };
	g_autoptr(GUdevDevice) dev = NULL;

	dev = create_single_dev (f, "P: /devices/dev1\n"
	                            "E: SUBSYSTEM=platform\n"
	                            "A: dytc_lapmode=1\n"
	                            "A: console=Y\\n\n"
	                            "E: ID_MODEL=KoolGadget");

	g_assert_cmpstrv (g_udev_device_get_sysfs_attr_keys (dev), expected);
}

int main(int argc, char **argv)
{
	setlocale (LC_ALL, NULL);
	g_test_init (&argc, &argv, NULL);

	g_test_add ("/gudev/uncached_sysfs_attr", Fixture, NULL,
	            fixture_setup,
	            test_uncached_sysfs_attr,
	            fixture_teardown);

	g_test_add ("/gudev/sysfs_attr_keys", Fixture, NULL,
	            fixture_setup,
	            test_sysfs_attr_keys,
	            fixture_teardown);

	return g_test_run ();
}
