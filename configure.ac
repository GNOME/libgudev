AC_PREREQ([2.64])
AC_INIT([libgudev],
        [230],
        [http://bugzilla.gnome.org/enter_bug.cgi?product=libgudev],
        [libgudev],
        [http://wiki.gnome.org/Projects/libgudev])
AC_CONFIG_SRCDIR([gudev/gudev.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])

AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE
AM_MAINTAINER_MODE([enable])
AM_INIT_AUTOMAKE([foreign 1.11 -Wall -Wno-portability silent-rules tar-pax no-dist-gzip dist-xz subdir-objects parallel-tests])
AM_SILENT_RULES([yes])
AC_CANONICAL_HOST
LT_PREREQ([2.2])
LT_INIT([disable-static])

AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_PROG_SED
AC_PROG_GREP
AC_PROG_AWK
AC_PROG_CC_C99
PKG_PROG_PKG_CONFIG

# gtkdocize greps for '^GTK_DOC_CHECK', so it needs to be on its own line
m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.18],[--flavour no-tmpl])],
         [AM_CONDITIONAL([ENABLE_GTK_DOC], [false])
          enable_gtk_doc=no])

m4_ifdef([GOBJECT_INTROSPECTION_CHECK], [
GOBJECT_INTROSPECTION_CHECK([1.31.1])
], [
   AM_CONDITIONAL([HAVE_INTROSPECTION], [false])
   enable_introspection=no])

PKG_CHECK_MODULES([LIBUDEV], [libudev >= 199])
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.22.0 gobject-2.0 >= 2.22.0 gio-2.0])

AC_CONFIG_FILES([
        Makefile
        gudev-1.0.pc
        docs/Makefile
        docs/version.xml
])

AC_OUTPUT
AC_MSG_RESULT([
        $PACKAGE_NAME $VERSION
        gintrospection:          ${enable_introspection}
        gtk-doc:                 ${enable_gtk_doc}

        prefix:                  ${prefix}
        exec_prefix:             ${exec_prefix}
        includedir:              ${includedir}
        libdir:                  ${libdir}

        CFLAGS:                  ${CFLAGS}
        CPPFLAGS:                ${CPPFLAGS}
        LDFLAGS:                 ${LDFLAGS}
])
