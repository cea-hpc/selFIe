TESTS = hello.exe selfie_hello.sh
check_PROGRAMS = hello.exe
check_SCRIPTS = selfie_hello.sh
hello_exe_SOURCES = hello.c


selfie_hello.sh: selfie_hello.sh.in hello.exe
	sed 's|@selfiepath@|'${top_srcdir}'|g' selfie_hello.sh.in > selfie_hello.sh
	chmod 750 selfie_hello.sh
