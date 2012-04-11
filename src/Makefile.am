bin_PROGRAMS = qinfo

qinfo_SOURCES = qinfo.c console.c window.c callbacks.c settings.c hyperlink.c open.c preferences.c about.c
qinfo_LDADD   = $(DEPS_LIBS)

AM_CPPFLAGS  = $(DEPS_CFLAGS) \
               -DVERSION=\""$(PACKAGE_VERSION)"\" \
               -DPIXMAPS_DIR=\""$(datadir)/pixmaps"\" \
               -DUI_DIR=\""$(pkgdatadir)/ui"\"