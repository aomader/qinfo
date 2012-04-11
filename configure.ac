AC_INIT(qinfo, 1.0)
AC_CONFIG_SRCDIR(src/qinfo.c)
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

AC_PROG_CC
AC_STDC_HEADERS
AC_PROG_INSTALL

if test "$GCC" = "yes"; then
	CFLAGS="$CFLAGS -Wall -std=c99"
fi

PKG_CHECK_MODULES(DEPS, gtk+-2.0 gobject-2.0 gmodule-2.0)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

AC_ARG_WITH(desktop-dir, [  --with-desktop-dir=PATH where to install desktop files (default=PREFIX/share)])

if test "x$with_desktop_dir" != x; then
	DESKTOP_DATADIR=$with_desktop_dir
else
	DESKTOP_DATADIR="$datadir"
fi

AM_CONDITIONAL(DESKTOP_DATADIR, test "x$with_desktop_dir" != xno)
AC_SUBST(DESKTOP_DATADIR)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 data/Makefile
                 data/desktop/Makefile
                 data/ui/Makefile])
AC_CONFIG_COMMANDS([default],[[ echo timestamp > stamp-h ]],[[]])

AC_OUTPUT
