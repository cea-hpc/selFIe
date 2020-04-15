TESTS = hello.exe \
	selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh 
check_PROGRAMS = hello.exe
check_SCRIPTS = selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh
hello_exe_SOURCES = hello.c
CLEANFILES = selfie.conf \
	selfie_hello.sh \
	selfie_genconf.sh \
	selfie_helloconf.sh \
	selfie_hellolog.sh \
	selfie_hellovar.sh \
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


