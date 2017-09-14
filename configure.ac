#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_define([selfie_major_version], [1])
m4_define([selfie_minor_version], [0])
m4_define([selfie_revision_version], [0])
m4_define([selfie_version], [selfie_major_version.selfie_minor_version])

AC_PREREQ([2.63])
AC_INIT([SELFIE], [selfie_version], [laurent.nguyen@cea.fr],[],[https://github.com/cea-hpc/selFIe])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([src/selfie.c])
AC_CONFIG_HEADERS([config.h])

# Version
AC_DEFINE([SELFIE_MAJOR_VERSION],[selfie_major_version],[Major version])
AC_DEFINE([SELFIE_MINOR_VERSION],[selfie_minor_version],[Minor version])
AC_DEFINE([SELFIE_REVISION_VERSION],[selfie_revision_version],[Revision version])
AC_DEFINE([SELFIE_VERSION],[selfie_version],[Version])

# use the C++ compiler for the following checks
AC_LANG([C])

# use Python
AM_PATH_PYTHON([2.6])

# Checks for programs.
AC_CONFIG_MACRO_DIRS([m4])
AC_PROG_CC([gcc cc])
AC_PROG_CXX([g++ c++])
AC_LANG_PUSH(C)
AC_LANG_POP(C)

AM_PROG_AR
AM_PROG_LIBTOOL
AC_PROG_INSTALL

# Checks for libraries.
AC_DISABLE_STATIC

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([regex.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([syslog.h])
AC_CHECK_HEADERS([sys/resource.h])
AC_CHECK_HEADERS([sys/stat.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([sys/types.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([dlfcn.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SSIZE_T

# Options
AC_ARG_ENABLE(debug,
 [  --enable-debug  Enable debug mode],
 [ AC_DEFINE([HAVE_DEBUG],[],[Enable debug mode]) ])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([regcomp])
AC_CHECK_FUNCS([setlocale])
AC_CHECK_FUNCS([strdup])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([getopt])
AC_CHECK_FUNCS([readlink])
AC_CHECK_FUNCS([stat])
AC_CHECK_FUNCS([strcpy])
AC_CHECK_FUNCS([strcat])
AC_CHECK_FUNCS([strncpy])
AC_CHECK_FUNCS([strncat])
AC_CHECK_FUNCS([strlen])
AC_CHECK_FUNCS([snprintf])
AC_CHECK_FUNCS([sprintf])
AC_CHECK_FUNCS([fprintf])
AC_CHECK_FUNCS([printf])
AC_CHECK_FUNCS([fopen])
AC_CHECK_FUNCS([fclose])
AC_CHECK_FUNCS([atof])
AC_CHECK_FUNCS([atoi])
AC_CHECK_FUNCS([strdup])
AC_CHECK_FUNCS([getenv])
AC_CHECK_FUNCS([regcomp])
AC_CHECK_FUNCS([regexec])
AC_CHECK_FUNCS([regfree])
AC_CHECK_FUNCS([openlog])
AC_CHECK_FUNCS([syslog])
AC_CHECK_FUNCS([closelog])

AC_CHECK_LIB([dl],[dlopen],[],[AC_MSG_ERROR([Unable to use libdl])])
AC_CHECK_LIB([dl],[dlsym],[],[AC_MSG_ERROR([Unable to use libdl])])
AC_CHECK_LIB([dl],[dlclose],[],[AC_MSG_ERROR([Unable to use libdl])])

# Check for libYAML

# introduce the optional configure parameter for a non-standard install prefix of libyaml
AC_ARG_WITH([libyaml],
    [AS_HELP_STRING([--with-libyaml=prefix],
        [Directory where to find libyaml (libYAML must have been compiled with --with-pic option)])],
    [LIBYAMLPATHSET=1],
    [LIBYAMLPATHSET=0])
	
# if optional parameter used, extend path flags for compliler and linker
if test $LIBYAMLPATHSET = 1 ; then
    # extend the compiler and linker flags according to the path set
    AM_CXXFLAGS="$AM_CXXFLAGS -I$with_libyaml/include"
    AM_CPPFLAGS="$AM_CPPFLAGS -I$with_libyaml/include"
    AM_CFLAGS="$AM_CFLAGS -I$with_libyaml/include"
    LIBS="$LIBS $with_libyaml/lib/libyaml.a"
    AC_MSG_CHECKING([libyaml prefix])
    AC_MSG_RESULT([$with_libyaml])
else
    # Otherwise, link to libyaml from system
    LIBS="$LIBS -lyaml"
fi

CPPFLAGS="$AM_CPPFLAGS $CPPFLAGS"
CXXFLAGS="$AM_CXXFLAGS $CXXFLAGS"
CFLAGS="$AM_CFLAGS $CFLAGS"

AC_CHECK_HEADERS([yaml.h], [], [AC_MSG_ERROR([Unable to find libyaml])])
AC_CHECK_FUNCS([yaml_parser_initialize], [], [AC_MSG_ERROR([Unable to use libyaml])])
AC_CHECK_FUNCS([yaml_parser_set_input_file], [], [AC_MSG_ERROR([Unable to use libyaml])])
AC_CHECK_FUNCS([yaml_parser_parse], [], [AC_MSG_ERROR([Unable to use libyaml])])
AC_CHECK_FUNCS([yaml_event_delete], [], [AC_MSG_ERROR([Unable to use libyaml])])
AC_CHECK_FUNCS([yaml_parser_delete], [], [AC_MSG_ERROR([Unable to use libyaml])])

# Check for POSIXIO

AC_ARG_WITH([posixio],
    [AS_HELP_STRING([--with-posixio=builtin|hook],
        [Enable POSIXIO profiling (default=hook)])],
    [ENABLEPOSIXIO=1],
    [ENABLEPOSIXIO=0])

if test $ENABLEPOSIXIO = 1; then
   AC_DEFINE([HAVE_POSIXIO],[],[Enable POSIXIO profiling])
   if test "x$with_posixio" = xbuiltin ; then
         AC_DEFINE([__SELFIE_POSIXIO_BUILTIN__],[],[Enable POSIXIO built-in hook])
   else
         AC_DEFINE([__SELFIE_POSIXIO_HOOK__],[],[Enable POSIXIO standard hook])
   fi
fi

# Check for MPI

AC_ARG_WITH([mpi],
    [AS_HELP_STRING([--with-mpi],
        [Enable MPI profiling])],
    [ENABLEMPI=1],
    [ENABLEMPI=0])

if test $ENABLEMPI = 1; then
   AC_DEFINE([HAVE_MPI],[],[Enable MPI profiling])
   AC_DEFINE([__SELFIE_MPI_BUILTIN__],[],[Enable MPI built-in hook])	
fi

# Check for PAPI

# introduce the optional configure parameter for a non-standard install prefix of PAPI
AC_ARG_WITH([papi],
    [AS_HELP_STRING([--with-papi=prefix],
        [Directory where to find PAPI (PAPI must have been compiled with shared library) ])],
    [LIBPAPIPATHSET=1],
    [LIBPAPIPATHSET=0])

OLD_AM_CXXFLAGS="$AM_CXXFLAGS"
OLD_AM_CPPFLAGS="$AM_CPPFLAGS"
OLD_AM_CFLAGS="$AM_CFLAGS"
OLD_LIBS="$LIBS"

# if optional parameter used, extend path flags for compliler and linker
if test $LIBPAPIPATHSET = 1 ; then
    if test "x$with_papi" != xno ; then
        OLD_AM_CXXFLAGS="$AM_CXXFLAGS"
        OLD_AM_CPPFLAGS="$AM_CPPFLAGS"
	OLD_AM_CFLAGS="$AM_CFLAGS"
        OLD_LIBS="$LIBS"
     	# Enable PAPI plugin
        AC_DEFINE([HAVE_PAPI],[],[Enable PAPI plugin])
	# Check system path for libpapi.so
        if test "x$with_papi" != xyes ; then
	   AC_MSG_CHECKING([PAPI prefix])
	   AC_MSG_RESULT([$with_papi])
	   PAPI_PREFIX="$with_papi"
	   AC_CHECK_FILE([$PAPI_PREFIX/include/papi.h], 
		      [PAPI_INCLUDE="$PAPI_PREFIX/include/papi.h"])
	   AC_CHECK_FILE([$PAPI_PREFIX/lib/libpapi.so], 
		      [LIBPAPI_PATH="$PAPI_PREFIX/lib"])
	   AC_CHECK_FILE([$PAPI_PREFIX/lib64/libpapi.so], 
		      [LIBPAPI_PATH="$PAPI_PREFIX/lib64"]) 
           AM_CXXFLAGS="$AM_CXXFLAGS -I$PAPI_PREFIX/include"
           AM_CPPFLAGS="$AM_CPPFLAGS -I$PAPI_PREFIX/include"
           AM_CFLAGS="$AM_CFLAGS -I$PAPI_PREFIX/include"
	   LIBS="$LIBS $LIBPAPI_PATH/libpapi.so"
	   AC_DEFINE_UNQUOTED([SELFIE_PAPILIBPATH],["$LIBPAPI_PATH"],[Enable directory where to find libpapi.so])
	else
	   LIBS="$LIBS -lpapi"
	   AC_DEFINE_UNQUOTED([SELFIE_PAPILIBPATH],["/usr/lib/x86_64-linux-gnu/"],[Enable directory where to find libpapi.so])
	fi

        AC_CHECK_FUNCS([PAPI_library_init], [], [AC_MSG_ERROR([Unable to use PAPI])])
        AC_CHECK_FUNCS([PAPI_library_init])
        AC_CHECK_FUNCS([PAPI_multiplex_init])
        AC_CHECK_FUNCS([PAPI_create_eventset])
        AC_CHECK_FUNCS([PAPI_event_name_to_code])
        AC_CHECK_FUNCS([PAPI_add_event])
        AC_CHECK_FUNCS([PAPI_start])
        AC_CHECK_FUNCS([PAPI_stop])
        AC_CHECK_FUNCS([PAPI_cleanup_eventset])
        AC_CHECK_FUNCS([PAPI_destroy_eventset])
	AC_DEFINE_UNQUOTED([SELFIE_PAPILIB],["libpapi.so"],[Name of PAPI library])
    fi
fi

#AM_CXXFLAGS="$OLD_AM_CXXFLAGS"
#AM_CPPFLAGS="$OLD_AM_CPPFLAGS"
#AM_CFLAGS="$OLD_AM_CFLAGS"
LIBS="$OLD_LIBS"

# Add math library
LIBS="$LIBS -lm"

# Plugins
AC_DEFINE([HAVE_DUMMY],[],[Enable dummy plugin])
AC_DEFINE([HAVE_DEFAULT],[],[Enable default plugin])

# distribute additional compiler and linker flags among Makefiles
# --> set and change these variables instead of CXXFLAGS or LDFLAGS (for user only)
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

# Set configuration parameters
AC_SUBST([CONFIGFILENAME],[selfie.conf])

AC_CONFIG_FILES([Makefile
	src/Makefile])

AC_OUTPUT
