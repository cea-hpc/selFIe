TESTS = hello selfie_hello.sh
check_PROGRAMS = hello
check_SCRIPTS = selfie_hello.sh
hello_SOURCES = hello.c


selfie_hello.sh: selfie_hello.sh.in hello
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_hello.sh.in > selfie_hello.sh
	chmod 750 selfie_hello.sh
