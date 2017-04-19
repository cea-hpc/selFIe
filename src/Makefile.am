lib_LTLIBRARIES = selfie.la
selfie_la_SOURCES = selfie.c selfie_tools.c \
	selfie_dummy.c \
	selfie_default.c \
	selfie_papi.c \
	selfie_posixio.cpp \
	selfie_mpi.cpp \
	selfie_mpiio.cpp
selfie_la_LDFLAGS = -module -avoid-version -shared
ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += -DCONFIGFILEPATH=\"${sysconfdir}\"
AM_CPPFLAGS += -DCONFIGFILENAME=\"${CONFIGFILENAME}\"

noinst_SCRIPTS = parse_posixio.py parse_mpi.py
CLEANFILES = selfie_posixio_functions.h selfie_mpi_functions.h selfie_mpiio_functions.h

selfie.c: ${top_srcdir}/include/selfie.h ${top_srcdir}/include/selfie_tools.h ${top_srcdir}/include/selfie_plugins.h

selfie_tools.c: ${top_srcdir}/include/selfie_tools.h

selfie_dummy.c: ${top_srcdir}/include/selfie_tools.h ${top_srcdir}/include/selfie_dummy.h

selfie_default.c: ${top_srcdir}/include/selfie_tools.h ${top_srcdir}/include/selfie_default.h

selfie_papi.c: ${top_srcdir}/include/selfie_tools.h ${top_srcdir}/include/selfie_papi.h

selfie_posixio.cpp: selfie_posixio_functions.h ${top_srcdir}/include/selfie_tools.h ${top_srcdir}/include/selfie_posixio.h

selfie_posixio_functions.h: $(srcdir)/parse_posixio.py ${top_srcdir}/etc/selfie_posixio_functions.txt
	$(PYTHON) $(srcdir)/parse_posixio.py -i ${top_srcdir}/etc/selfie_posixio_functions.txt -o $@

selfie_mpi.cpp: selfie_mpi_functions.h ${top_srcdir}/include/selfie_tools.h ${top_srcdir}/include/selfie_mpi.h

selfie_mpi_functions.h: $(srcdir)/parse_mpi.py ${top_srcdir}/etc/selfie_mpi_functions.txt
	$(PYTHON) $(srcdir)/parse_mpi.py -i ${top_srcdir}/etc/selfie_mpi_functions.txt -o $@

selfie_mpiio.cpp: selfie_mpiio_functions.h ${top_srcdir}/include/selfie_tools.h ${top_srcdir}/include/selfie_mpiio.h

selfie_mpiio_functions.h: $(srcdir)/parse_mpi.py ${top_srcdir}/etc/selfie_mpiio_functions.txt
	$(PYTHON) $(srcdir)/parse_mpi.py -n mpiio -i ${top_srcdir}/etc/selfie_mpiio_functions.txt -o $@
