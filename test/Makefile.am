TESTS = hello.exe \
	hello_omp.exe \
	selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh \
	selfie_hello_omp.sh
check_PROGRAMS = hello.exe \
	hello_omp.exe
check_SCRIPTS = selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh \
	selfie_hello_omp.sh
hello_exe_SOURCES = hello.c
hello_omp_exe_SOURCES = hello_omp.c
hello_omp_exe_CFLAGS = $(OPENMP_CFLAGS) $(CFLAGS)
hello_omp_exe_LDFLAGS = $(OPENMP_CFLAGS) $(LDFLAGS)

CLEANFILES = selfie.conf \
	selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh \
	selfie_hello_omp.sh \
	*.out *.err *~

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

selfie_hello_omp.sh: selfie_hello_omp.sh.in hello_omp.exe
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_hello_omp.sh.in > selfie_hello_omp.sh
	chmod 750 selfie_hello_omp.sh


