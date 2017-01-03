dnl Every other copy of the package version number gets its value from here
AC_INIT(libr4d, 1.0, b.spranger@linutronix.de)
AM_INIT_AUTOMAKE
AC_USE_SYSTEM_EXTENSIONS

dnl create a config.h file (Automake will add -DHAVE_CONFIG_H)
AM_CONFIG_HEADER(config.h)

AC_SUBST(VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

AC_CANONICAL_HOST
AC_CANONICAL_BUILD

dnl Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(locale.h)

AC_CHECK_HEADER(argp.h,,AC_MSG_ERROR(Cannot continue: argp.h not found))

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

dnl pkg-conf section
PKG_CHECK_MODULES(GSOAP, gsoap)
AC_SUBST(GSOAP_CFLAGS)
AC_SUBST(GSOAP_LIBS)

PKG_CHECK_MODULES(UUID, uuid)
AC_SUBST(UUID_CFLAGS)
AC_SUBST(UUID_LIBS)

PKG_CHECK_MODULES(GLIB, glib-2.0 > 2.12)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

dnl i18n makros.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])

AC_OUTPUT([Makefile
	libr4d.doxygen
	libr4d.pc
	libr4d-uninstalled.pc
	po/Makefile.in])
