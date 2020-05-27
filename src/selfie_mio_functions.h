// Copyright (C) 2015-2020 CEA/DAM
// Copyright (C) 2015-2020 Laurent Nguyen <laurent.nguyen@cea.fr>
//
// This file is part of SelFIe.
//
// This software is governed by the CeCILL-C license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL-C
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".

///
/// @file   selfie_mio_functions.h
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Wed Apr 22 12:00:00 2020
///
/// @brief  Header file for plugin selfie_mio
///
///
///
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <cstring>
#include <cstdio>
#include <execinfo.h>
#include <dlfcn.h>
#include <cstdarg>
#include <fenv.h>
#include <syslog.h>
#include <time.h>

#pragma STDC FENV_ACCESS ON

typedef void (*function_type)(...);

/// \brief Total number of MPI functions
#define N_MIO_FUNCTIONS 2

/// \brief Return a string containing name of functions
/// \param[in] i Index
/// \return   Return a string containing name of functions
///
char *selfie_get_mio_function_name(int i)
{
  char const *mio_functions_name[] = {"mio_init", "mio_fini", NULL};
  return strdup(mio_functions_name[i]);
};

/// \brief Array of pointers of functions
function_type selfie_mio_orig_pointer_functions[N_MIO_FUNCTIONS] = {NULL};

/// \brief Array of pointers of functions
function_type *selfie_mio_pointer_functions = selfie_mio_orig_pointer_functions;

extern "C"
{
  /// \brief mio_ini
  ///
  /// \param ...
  /// \return int
  ///
  /// \details
  ///
  int mio_init(...)
  {
    double f_start = 0.0;
    int ap_except = 0;
    int val = (int)0;
    char miolog[1024];
    int function_number = 0;

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_mio_pointer_functions[function_number];
    if (selfie_function == NULL)
    {
      selfie_function = (function_type)dlsym(RTLD_NEXT, "mio_init");
      if (selfie_function == NULL)
	return val;
    }
    selfie_mio_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_mio_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf(miolog,
	    "%s \"timestamp\": %llu, \"function\": \"%s\", \"wtime\": %0.2f }",
	    selfie_mio_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_mio_global_data[function_number].function_time);
    selfie_trace_log(miolog, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);

    return val;
  };

  /// \brief mio_fini
  ///
  /// \param ...
  /// \return int
  ///
  /// \details
  ///
  void mio_fini(...)
  {
    double f_start = 0.0;
    int ap_except = 0;
    int val = (int)0;
    char miolog[1024];
    int function_number = 1;

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_mio_pointer_functions[function_number];
    if (selfie_function == NULL)
    {
      selfie_function = (function_type)dlsym(RTLD_NEXT, "mio_fini");
    }
    selfie_mio_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_mio_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf(miolog,
	    "%s \"timestamp\": %llu, \"function\": \"%s\", \"wtime\": %0.2f }",
	    selfie_mio_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_mio_global_data[function_number].function_time);
    selfie_trace_log(miolog, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);
  };
}
