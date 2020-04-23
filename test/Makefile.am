TESTS = hello.exe \
	selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh \
	mio.exe \
	selfie_mio.sh
check_PROGRAMS = hello.exe mio.exe
check_SCRIPTS = selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh \
	selfie_mio.sh
hello_exe_SOURCES = hello.c
#mio_exe_SOURCES = mio.c
#mio_exe_LDFLAGS = -L.libs -lselfie_mio_test
lib_LTLIBRARIES = libselfie_mio_test.la
libselfie_mio_test_la_SOURCES = selfie_mio_test.c
libselfie_mio_test_la_LDFLAGS = -module -avoid-version -shared

AM_CFLAGS += -Wall
AM_CXXFLAGS += -Wall
CLEANFILES = selfie.conf \
	selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh \
	selfie_mio.sh \
	*.out *.err *~
AM_CPPFLAGS += -I$(top_srcdir)/include

mio.exe: libselfie_mio_test.la mio.c
	$(CC) -I${top_srcdir}/include -o $@ mio.c -L.libs -Wl,-rpath,.libs -lselfie_mio_test

selfie_hello.sh: selfie_hello.sh.in hello.exe
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_hello.sh.in > selfie_hello.sh
	chmod 750 selfie_hello.sh

selfie_genconf.sh: selfie_genconf.sh.in
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_genconf.sh.in > selfie_genconf.sh
	chmod 750 selfie_genconf.sh

selfie_helloconf.sh: selfie_helloconf.sh.in hello.exe
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_helloconf.sh.in > selfie_helloconf.sh
	chmod 750 selfie_helloconf.sh

selfie_hellolog.sh: selfie_hellolog.sh.in hello.exe
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_hellolog.sh.in > selfie_hellolog.sh
	chmod 750 selfie_hellolog.sh

selfie_hellovar.sh: selfie_hellovar.sh.in hello.exe
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_hellovar.sh.in > selfie_hellovar.sh
	chmod 750 selfie_hellovar.sh

selfie_mio.sh: selfie_mio.sh.in mio.exe
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_mio.sh.in > selfie_mio.sh
	chmod 750 selfie_mio.sh


