
SUBDIRS=harness leak

if INCLUDE_BROWSER_MANAGED_CODE
SUBDIRS += 2.0
endif

DIST_SUBDIRS = $(SUBDIRS)

if INCLUDE_TESTING
TESTING = true
else
TESTING = echo ERROR: Test harness is disabled by configuration; false
endif

check-testing:
	$(TESTING)

install-test-plugin: check-testing
	$(MAKE) $(AM_MAKEFLAGS) -C harness/shocker install-test-plugin

run-unit-tests:
	$(MAKE) -C 2.0/moon-unit test

run-unit-tests-harness:
	$(MAKE) -C 2.0/moon-unit
	MOONLIGHT_UNIT_HARNESS_LOG_DIRECTORY=$(srcdir)/test-run-data DRTLIST=2.0/moon-unit/site/drtlist.xml $(MAKE) run-xephyr

dist-hook:
	cp -a $(srcdir)/xaml $(distdir)/xaml
	find $(distdir)/xaml -name .svn | xargs rm -rf
	cp -a $(srcdir)/media $(distdir)/media
	find $(distdir)/media -name .svn | xargs rm -rf

gendarme:
	gendarme --html gendarme.html --config gendarme.xml $(prefix)/lib/moonlight/plugin/*.dll --ignore gendarme.ignore

if HAVE_GLX
noinst_PROGRAMS = effects projections
endif

effects_SOURCES= effect-test.cpp

effects_LDADD = $(MOON_PROG_LIBS)

effects_CPPFLAGS = $(MOON_PROG_CFLAGS) $(GALLIUM_CFLAGS) $(CURL_CFLAGS) -I$(top_srcdir)/src/ -I$(top_srcdir)/src/asf

projections_SOURCES= projection-test.cpp

projections_LDADD = $(MOON_PROG_LIBS)

projections_CPPFLAGS = $(MOON_PROG_CFLAGS) $(GALLIUM_CFLAGS) $(CURL_CFLAGS) -I$(top_srcdir)/src/ -I$(top_srcdir)/src/asf

