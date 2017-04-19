// Copyright (C) 2015-2017 CEA/DAM
// Copyright (C) 2015-2017 Laurent Nguyen <laurent.nguyen@cea.fr>
//
// This file is part of SelFIe.
//
// This software is governed by the CeCILL-C license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL-C
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".

///
/// @file   selfie_mpi.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Main file for plugin selfie_mpi
///
///
///
#include "config.h"
#ifdef HAVE_MPI

#include <cstdio>

extern "C" {
#include "selfie_tools.h"
#include "selfie_mpiio.h"
}
/// \typedef mpiio_plugin_global_data
/// \brief   mpiio_plugin_global_data of struct Mpiio_plugin_global_data
///
/// \details Structure for output parameter of plugins
///
typedef struct Mpiio_plugin_global_data
{
  /// Rank
  int rank;
  /// Size
  int nproc;
  /// number of MPIIO function calls
  unsigned long long int function_count;
  /// duration of MPIIO function calls
  double function_time;
  /// average size of MPIIO function calls
  double function_size;
} mpiio_plugin_global_data;

extern mpiio_plugin_global_data selfie_mpiio_global_data[];
extern mpiio_plugin_global_data selfie_mpiio_global_nproc_rank;

#include "selfie_mpiio_functions.h"

/// \brief mpiio plugin data (GLOBAL)
mpiio_plugin_global_data selfie_mpiio_global_data[N_MPIIO_FUNCTIONS];

/// \brief mpiio plugin data (GLOBAL)
mpiio_plugin_global_data selfie_mpiio_global_nproc_rank;

/// \brief Get MPIIO version and lib version
int selfie_mpiio_get_version(float *mpiio_version, char mpiio_libversion[])
{
  int (*func_mpiio_get_version)(int *, int *);
  int (*func_mpiio_get_libversion)(char *, int *);

  int mpiio_majorversion = 0;
  int mpiio_subversion = 0;

  char lib_version[2048] = "";
  int len = 0;

  // Known MPIIO libraries (30 characters max)
  char const *mpiio_list[] = {"open mpi", "mvapich2", "intel", "mpich", NULL};
  char const *it = NULL;
  int i = 0;
  char const *match = NULL;
  int flag = 0;
  int return_value = EXIT_FAILURE;

  *mpiio_version = 0.0;
  mpiio_libversion[0] = '\0';

  func_mpiio_get_version = NULL;
  func_mpiio_get_libversion = NULL;

  func_mpiio_get_version =
      (int (*)(int *, int *))dlsym(RTLD_NEXT, "MPI_Get_version");

  if (func_mpiio_get_version != NULL)
  {
    func_mpiio_get_version(&mpiio_majorversion, &mpiio_subversion);
    *mpiio_version = mpiio_majorversion + 0.1 * mpiio_subversion;
    return_value = EXIT_SUCCESS;
    if (*mpiio_version > 2.)
    {
      func_mpiio_get_libversion =
	  (int (*)(char *, int *))dlsym(RTLD_NEXT, "MPI_Get_library_version");
      if (func_mpiio_get_libversion != NULL)
      {
	func_mpiio_get_libversion(lib_version, &len);
	i = 0;
	it = mpiio_list[i];
	while ((flag == 0) && (it != NULL))
	{
	  if (strcasestr(lib_version, it))
	  {
	    flag = 1;
	    match = it;
	  }
	  i++;
	  it = mpiio_list[i];
	}
	if (flag != 0)
	{
	  strncpy(mpiio_libversion, match, 32);
	}
      }
    }
  }
  return return_value;
}

extern "C" {

/// \details   preInitialize here all variables for plugin
int selfie_plugin_mpiio_pre()
{
  int i = 0;
  for (i = 0; i < N_MPIIO_FUNCTIONS; i++)
  {
    char *tmp = selfie_get_mpiio_function_name(N_MPIIO_FUNCTIONS + i);
    selfie_mpiio_orig_pointer_functions[i] =
	(function_type)dlsym(RTLD_NEXT, tmp);
    free(tmp);
  }
  return EXIT_SUCCESS;
};

/// \details   Initialize here all variables for plugin
int selfie_plugin_mpiio_init(params_in *in, params_out *out)
{
  int i = 0;
  int enable = 1;
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  enable = selfie_getenv("SELFIE_NOMPIIO");
  if (in->enable == 0)
    enable = 0;

  for (i = 0; i < N_MPIIO_FUNCTIONS; i++)
  {
    selfie_mpiio_global_data[i].function_count = (unsigned long long int)0;
    selfie_mpiio_global_data[i].function_time = 0.0;
    selfie_mpiio_global_data[i].function_size = 0.0;
  }
  selfie_mpiio_global_nproc_rank.rank = 0;
  selfie_mpiio_global_nproc_rank.nproc = 1;

  return EXIT_SUCCESS;
};

/// \details   Release here all variables for plugin
int selfie_plugin_mpiio_finalize(params_in *in, params_out *out)
{
  int i = 0;
  double mpiio_wtime = 0.0;
  unsigned long long int mpiio_count = (unsigned long long int)0;
  float mpiio_version = 0.0;
  char mpiio_libversion[32] = "";
  double mpiio_size = 0.0;
  int mpiio_print = 0;
  char *tmp_string = NULL;
  char output[OUTPUT_ROWMAX] = "";
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  if (in->enable != 0)
  {
    for (i = 0; i < N_MPIIO_FUNCTIONS; i++)
    {
      mpiio_wtime += selfie_mpiio_global_data[i].function_time;
      mpiio_count += selfie_mpiio_global_data[i].function_count;
      mpiio_size += selfie_mpiio_global_data[i].function_size;
    }

    selfie_json_double_to_log(out, "mpiio_time", mpiio_wtime);
    selfie_json_llu_to_log(out, "mpiio_count", mpiio_count);
    // selfie_json_int_to_log(out,"mpiio_rank",
    // 				   selfie_mpiio_global_nproc_rank.rank);
    // selfie_json_int_to_log(out,"mpiio_nproc",
    // 				   selfie_mpiio_global_nproc_rank.nproc);
    if (selfie_mpiio_get_version(&mpiio_version, mpiio_libversion) ==
	EXIT_SUCCESS)
    {
      selfie_json_double_to_log(out, "mpiio_version", (double)mpiio_version);
      if (strlen(mpiio_libversion) > 0)
      {
	selfie_json_string_to_log(out, "mpiio_libversion", mpiio_libversion);
      }
    }
  }
  mpiio_print = !selfie_getenv("SELFIE_MPIIO_PRINT");
  output[OUTPUT_ROWMAX - 1] = '\0';
  if (mpiio_print != 0)
  {
    snprintf(output, OUTPUT_ROWMAX - 1, "\n=== SELFIE MPIIO DETAILS ===\n\n");
    selfie_strcat_output_to_params_out(out, output);
    snprintf(output, OUTPUT_ROWMAX - 1, "%20s\t%12s\t%12s\n\n", "Function",
	     "Count", "Time");
    selfie_strcat_output_to_params_out(out, output);
    for (i = 0; i < N_MPIIO_FUNCTIONS; i++)
    {
      if (selfie_mpiio_global_data[i].function_count > 0)
      {
	tmp_string = selfie_get_mpiio_function_name(i);
	snprintf(output, OUTPUT_ROWMAX - 1, "%20s\t%12llu\t%10.2lf\n",
		 tmp_string, selfie_mpiio_global_data[i].function_count,
		 selfie_mpiio_global_data[i].function_time);
	selfie_strcat_output_to_params_out(out, output);
	free(tmp_string);
      }
    }
    snprintf(output, OUTPUT_ROWMAX - 1, "\n");
    selfie_strcat_output_to_params_out(out, output);
  }

  return EXIT_SUCCESS;
};

/// \brief structure for plugin mpiio
plugins_functions selfie_plugin_mpiio = {selfie_plugin_mpiio_pre,
					 selfie_plugin_mpiio_init,
					 selfie_plugin_mpiio_finalize};
}
#endif
