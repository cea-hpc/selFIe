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
/// @file   selfie_maestro_functions.h
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Wed Apr 22 12:00:00 2020
///
/// @brief  Header file for plugin selfie_maestro
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
#define N_MAESTRO_FUNCTIONS 6

/// \brief Return a string containing name of functions
/// \param[in] i Index
/// \return   Return a string containing name of functions
///
char *selfie_get_maestro_function_name(int i)
{
  char const *maestro_functions_name[] = {
     "mstro_init",
     "mstro_finalize",
     "mstro_cdo_declare",
     "mstro_cdo_offer",
     "mstro_cdo_withdraw",
     "mstro_cdo_dispose",
     NULL};
  return strdup(maestro_functions_name[i]);
};

/// \brief Array of pointers of functions
function_type selfie_maestro_orig_pointer_functions[N_MAESTRO_FUNCTIONS] = {NULL};

/// \brief Array of pointers of functions
function_type *selfie_maestro_pointer_functions = selfie_maestro_orig_pointer_functions;

extern "C"
{
  /// \brief mstro_init
  ///
  /// \param ...
  /// \return int
  ///
  /// \details
  ///
  int mstro_init(...)
  {
    double f_start = 0.0;
    int ap_except = 0;
    int val = (int)0;
    char maestrolog[1024];
    int function_number = 0;

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_maestro_pointer_functions[function_number];
    if (selfie_function == NULL)
    {
      selfie_function = (function_type)dlsym(RTLD_NEXT, "mstro_init");
      if (selfie_function == NULL)
	return val;
    }
    selfie_maestro_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_maestro_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf(maestrolog,
	    "%s \"timestamp\": %llu, \"function\": \"%s\", \"wtime\": %0.2f }",
	    selfie_maestro_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_maestro_global_data[function_number].function_time);
    selfie_trace_log(maestrolog, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);

    return val;
  };

  /// \brief mstro_finalize
  ///
  /// \param ...
  /// \return int
  ///
  /// \details
  ///
  int mstro_finalize(...)
  {
    double f_start = 0.0;
    int ap_except = 0;
    int val = (int)0;
    char maestrolog[1024];
    int function_number = 1;

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_maestro_pointer_functions[function_number];
    if (selfie_function == NULL)
    {
      selfie_function = (function_type)dlsym(RTLD_NEXT, "mstro_finalize");
      if (selfie_function == NULL)
	return val;
    }
    selfie_maestro_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_maestro_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf(maestrolog,
	    "%s \"timestamp\": %llu, \"function\": \"%s\", \"wtime\": %0.2f }",
	    selfie_maestro_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_maestro_global_data[function_number].function_time);
    selfie_trace_log(maestrolog, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);

    return val;
  };

  /// \brief mstro_cdo_declare
  ///
  /// \param ...
  /// \return int
  ///
  /// \details
  ///
  int mstro_cdo_declare(...)
  {
    double f_start = 0.0;
    int ap_except = 0;
    int val = (int)0;
    char maestrolog[1024];
    int function_number = 2;

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_maestro_pointer_functions[function_number];
    if (selfie_function == NULL)
    {
      selfie_function = (function_type)dlsym(RTLD_NEXT, "mstro_cdo_declare");
      if (selfie_function == NULL)
	return val;
    }
    selfie_maestro_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_maestro_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf(maestrolog,
	    "%s \"timestamp\": %llu, \"function\": \"%s\", \"wtime\": %0.2f }",
	    selfie_maestro_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_maestro_global_data[function_number].function_time);
    selfie_trace_log(maestrolog, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);

    return val;
  };

  /// \brief mstro_cdo_offer
  ///
  /// \param ...
  /// \return int
  ///
  /// \details
  ///
  int mstro_cdo_offer(...)
  {
    double f_start = 0.0;
    int ap_except = 0;
    int val = (int)0;
    char maestrolog[1024];
    int function_number = 3;

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_maestro_pointer_functions[function_number];
    if (selfie_function == NULL)
    {
      selfie_function = (function_type)dlsym(RTLD_NEXT, "mstro_cdo_offer");
      if (selfie_function == NULL)
	return val;
    }
    selfie_maestro_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_maestro_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf(maestrolog,
	    "%s \"timestamp\": %llu, \"function\": \"%s\", \"wtime\": %0.2f }",
	    selfie_maestro_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_maestro_global_data[function_number].function_time);
    selfie_trace_log(maestrolog, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);

    return val;
  };

  /// \brief mstro_cdo_withdraw
  ///
  /// \param ...
  /// \return int
  ///
  /// \details
  ///
  int mstro_cdo_withdraw(...)
  {
    double f_start = 0.0;
    int ap_except = 0;
    int val = (int)0;
    char maestrolog[1024];
    int function_number = 4;

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_maestro_pointer_functions[function_number];
    if (selfie_function == NULL)
    {
      selfie_function = (function_type)dlsym(RTLD_NEXT, "mstro_cdo_withdraw");
      if (selfie_function == NULL)
	return val;
    }
    selfie_maestro_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_maestro_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf(maestrolog,
	    "%s \"timestamp\": %llu, \"function\": \"%s\", \"wtime\": %0.2f }",
	    selfie_maestro_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_maestro_global_data[function_number].function_time);
    selfie_trace_log(maestrolog, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);

    return val;
  };

  /// \brief mstro_cdo_dispose
  ///
  /// \param ...
  /// \return int
  ///
  /// \details
  ///
  int mstro_cdo_dispose(...)
  {
    double f_start = 0.0;
    int ap_except = 0;
    int val = (int)0;
    char maestrolog[1024];
    int function_number = 5;

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_maestro_pointer_functions[function_number];
    if (selfie_function == NULL)
    {
      selfie_function = (function_type)dlsym(RTLD_NEXT, "mstro_cdo_dispose");
      if (selfie_function == NULL)
	return val;
    }
    selfie_maestro_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_maestro_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf(maestrolog,
	    "%s \"timestamp\": %llu, \"function\": \"%s\", \"wtime\": %0.2f }",
	    selfie_maestro_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_maestro_global_data[function_number].function_time);
    selfie_trace_log(maestrolog, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);

    return val;
  };


}
