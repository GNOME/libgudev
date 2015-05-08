# ------------------------------------------------------------------------------
# Common variables

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AUTOMAKE_OPTIONS = color-tests parallel-tests
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc
SUBDIRS = .
lib_LTLIBRARIES =
noinst_DATA =
MANPAGES =
CLEANFILES = $(BUILT_SOURCES)
BUILT_SOURCES =

GCC_COLORS ?= 'ooh, shiny!'
export GCC_COLORS

# remove targets if the command fails
.DELETE_ON_ERROR:

# keep intermediate files
.SECONDARY:

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA =

AM_MAKEFLAGS = --no-print-directory
AM_CPPFLAGS =
AM_CFLAGS =

EXTRA_DIST = \
	README \
	COPYING \
	NEWS

# ------------------------------------------------------------------------------
# Version numbers

LIBGUDEV_CURRENT=2
LIBGUDEV_REVISION=0
LIBGUDEV_AGE=2

# ------------------------------------------------------------------------------
# libgudev

libgudev_includedir = $(includedir)/gudev-1.0/gudev

libgudev_include_HEADERS = \
	gudev/gudev.h \
	gudev/gudevenums.h \
	gudev/gudevenumtypes.h \
	gudev/gudevtypes.h \
	gudev/gudevclient.h \
	gudev/gudevdevice.h \
	gudev/gudevenumerator.h

lib_LTLIBRARIES += \
	libgudev-1.0.la

pkgconfig_DATA += \
	gudev-1.0.pc

EXTRA_DIST += \
	gudev-1.0.pc.in \
	gudev/gudevmarshal.list \
	gudev/gudevenumtypes.h.template \
	gudev/gudevenumtypes.c.template \
	scripts/gjs-example.js \
	scripts/seed-example-enum.js \
	scripts/seed-example.js

libgudev_1_0_la_SOURCES = \
	libgudev-1.0.sym \
	gudev/gudevenums.h \
	gudev/gudevenumtypes.h \
	gudev/gudevenumtypes.h\
	gudev/gudevtypes.h \
	gudev/gudevclient.h \
	gudev/gudevclient.c \
	gudev/gudevdevice.h \
	gudev/gudevdevice.c \
	gudev/gudevenumerator.h \
	gudev/gudevenumerator.c \
	gudev/gudevprivate.h

nodist_libgudev_1_0_la_SOURCES = \
	gudev/gudevmarshal.h \
	gudev/gudevmarshal.c \
	gudev/gudevenumtypes.h \
	gudev/gudevenumtypes.c

BUILT_SOURCES += \
	$(nodist_libgudev_1_0_la_SOURCES)

libgudev_1_0_la_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(top_builddir)/gudev \
	-I$(top_srcdir)/gudev \
	-D_POSIX_PTHREAD_SEMANTICS \
	-D_REENTRANT \
	-D_GUDEV_COMPILATION \
	-DG_LOG_DOMAIN=\"GUdev\"

libgudev_1_0_la_CFLAGS = \
	$(AM_CFLAGS) \
	-fvisibility=default \
	$(LIBUDEV_CFLAGS) \
	$(GLIB_CFLAGS)

libgudev_1_0_la_LIBADD = \
	$(LIBUDEV_LIBS) \
	$(GLIB_LIBS)

libgudev_1_0_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-version-info $(LIBGUDEV_CURRENT):$(LIBGUDEV_REVISION):$(LIBGUDEV_AGE) \
	-export-dynamic \
	-no-undefined \
	-Wl,--version-script=$(top_srcdir)/libgudev-1.0.sym

gudev/gudevmarshal.h: gudev/gudevmarshal.list
	$(AM_V_at)$(MKDIR_P) $(dir $@)
	$(AM_V_GEN)glib-genmarshal $< --prefix=g_udev_marshal --header > $@

gudev/gudevmarshal.c: gudev/gudevmarshal.list
	$(AM_V_at)$(MKDIR_P) $(dir $@)
	$(AM_V_GEN)echo '#include "gudevmarshal.h"' > $@ && \
	glib-genmarshal $< --prefix=g_udev_marshal --body >> $@

gudev/gudevenumtypes.%: gudev/gudevenumtypes.%.template gudev/gudevenums.h
	$(AM_V_at)$(MKDIR_P) $(dir $@)
	$(AM_V_GEN)glib-mkenums --template $^ > $@

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)

gudev/GUdev-1.0.gir: libgudev-1.0.la

gudev_GUdev_1_0_gir_INCLUDES = GObject-2.0

gudev_GUdev_1_0_gir_CFLAGS = \
	$(AM_CFLAGS) \
	$(INCLUDES) \
	-D_GUDEV_COMPILATION \
	-D_GUDEV_WORK_AROUND_DEV_T_BUG \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	-I$(top_srcdir)/gudev \
	-I$(top_builddir)/gudev

gudev_GUdev_1_0_gir_LIBS = libgudev-1.0.la

gudev_GUdev_1_0_gir_SCANNERFLAGS = \
	--pkg-export=gudev-1.0 \
	--warn-all

gudev_GUdev_1_0_gir_FILES = \
	gudev/gudev.h \
	gudev/gudevtypes.h \
	gudev/gudevenums.h \
	gudev/gudevenumtypes.h \
	gudev/gudevclient.h \
	gudev/gudevdevice.h \
	gudev/gudevenumerator.h \
	gudev/gudevclient.c \
	gudev/gudevdevice.c \
	gudev/gudevenumerator.c

INTROSPECTION_GIRS = gudev/GUdev-1.0.gir
INTROSPECTION_SCANNER_ARGS = --c-include=gudev/gudev.h

girdir = $(datadir)/gir-1.0
gir_DATA = \
	gudev/GUdev-1.0.gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = \
	gudev/GUdev-1.0.typelib

CLEANFILES += $(gir_DATA) $(typelibs_DATA)
endif # HAVE_INTROSPECTION

# ------------------------------------------------------------------------------
# docs

if ENABLE_GTK_DOC
SUBDIRS += docs
endif

#docs/html:
#	$(AM_V_at)$(MKDIR_P) $(dir $@)
#	$(AM_V_LN)$(LN_S) -f ../gudev/html $@
