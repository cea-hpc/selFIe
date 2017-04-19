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
#include "selfie_mpi.h"
}
/// \typedef mpi_plugin_global_data
/// \brief   mpi_plugin_global_data of struct Mpi_plugin_global_data
///
/// \details Structure for output parameter of plugins
///
typedef struct Mpi_plugin_global_data
{
  /// Rank
  int rank;
  /// Size
  int nproc;
  /// number of MPI function calls
  unsigned long long int function_count;
  /// duration of MPI function calls
  double function_time;
  /// average size of MPI function calls
  double function_size;
} mpi_plugin_global_data;

extern mpi_plugin_global_data selfie_mpi_global_data[];
extern mpi_plugin_global_data selfie_mpi_global_nproc_rank;

#include "selfie_mpi_functions.h"

/// \brief mpi plugin data (GLOBAL)
mpi_plugin_global_data selfie_mpi_global_data[N_MPI_FUNCTIONS];

/// \brief mpi plugin data (GLOBAL)
mpi_plugin_global_data selfie_mpi_global_nproc_rank;

/// \brief Get MPI version and lib version
int selfie_mpi_get_version(float *mpi_version, char mpi_libversion[])
{
  int (*func_mpi_get_version)(int *, int *);
  int (*func_mpi_get_libversion)(char *, int *);

  int mpi_majorversion = 0;
  int mpi_subversion = 0;

  char lib_version[2048] = "";
  int len = 0;

  // Known MPI libraries (30 characters max)
  char const *mpi_list[] = {"open mpi", "mvapich2", "intel", "mpich", NULL};
  char const *it = NULL;
  int i = 0;
  char const *match = NULL;
  int flag = 0;
  int return_value = EXIT_FAILURE;

  *mpi_version = 0.0;
  mpi_libversion[0] = '\0';

  func_mpi_get_version = NULL;
  func_mpi_get_libversion = NULL;

  func_mpi_get_version =
      (int (*)(int *, int *))dlsym(RTLD_NEXT, "MPI_Get_version");

  if (func_mpi_get_version != NULL)
  {
    func_mpi_get_version(&mpi_majorversion, &mpi_subversion);
    *mpi_version = mpi_majorversion + 0.1 * mpi_subversion;
    return_value = EXIT_SUCCESS;
    if (*mpi_version > 2.)
    {
      func_mpi_get_libversion =
	  (int (*)(char *, int *))dlsym(RTLD_NEXT, "MPI_Get_library_version");
      if (func_mpi_get_libversion != NULL)
      {
	func_mpi_get_libversion(lib_version, &len);
	i = 0;
	it = mpi_list[i];
	while ((flag == 0) && (it != NULL))
	{
	  if (strcasestr(lib_version, it))
	  {
	    flag = 1;
	    match = it;
	  }
	  i++;
	  it = mpi_list[i];
	}
	if (flag != 0)
	{
	  strncpy(mpi_libversion, match, 32);
	}
      }
    }
  }
  return return_value;
}

extern "C" {

/// \details   preInitialize here all variables for plugin
int selfie_plugin_mpi_pre()
{
  int i = 0;
  for (i = 0; i < N_MPI_FUNCTIONS; i++)
  {
    char *tmp = selfie_get_mpi_function_name(N_MPI_FUNCTIONS + i);
    selfie_mpi_orig_pointer_functions[i] = (function_type)dlsym(RTLD_NEXT, tmp);
    free(tmp);
  }
  return EXIT_SUCCESS;
};

/// \details   Initialize here all variables for plugin
int selfie_plugin_mpi_init(params_in *in, params_out *out)
{
  int i = 0;
  int enable = 1;
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  enable = selfie_getenv("SELFIE_NOMPI");
  if (in->enable == 0)
    enable = 0;

  for (i = 0; i < N_MPI_FUNCTIONS; i++)
  {
    selfie_mpi_global_data[i].function_count = (unsigned long long int)0;
    selfie_mpi_global_data[i].function_time = 0.0;
    selfie_mpi_global_data[i].function_size = 0.0;
  }
  selfie_mpi_global_nproc_rank.rank = 0;
  selfie_mpi_global_nproc_rank.nproc = 1;

  return EXIT_SUCCESS;
};

/// \details   Release here all variables for plugin
int selfie_plugin_mpi_finalize(params_in *in, params_out *out)
{
  int i = 0;
  double mpi_wtime = 0.0;
  unsigned long long int mpi_count = (unsigned long long int)0;
  float mpi_version = 0.0;
  char mpi_libversion[32] = "";
  double mpi_size = 0.0;
  int mpi_print = 0;
  char *tmp_string = NULL;
  char output[OUTPUT_ROWMAX] = "";
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  if (in->enable != 0)
  {
    for (i = 0; i < N_MPI_FUNCTIONS; i++)
    {
      mpi_wtime += selfie_mpi_global_data[i].function_time;
      mpi_count += selfie_mpi_global_data[i].function_count;
      mpi_size += selfie_mpi_global_data[i].function_size;
    }

    selfie_json_double_to_log(out, "mpi_time", mpi_wtime);
    selfie_json_llu_to_log(out, "mpi_count", mpi_count);
    // selfie_json_int_to_log(out,"mpi_rank",
    // 				   selfie_mpi_global_nproc_rank.rank);
    // selfie_json_int_to_log(out,"mpi_nproc",
    // 				   selfie_mpi_global_nproc_rank.nproc);
    if (selfie_mpi_get_version(&mpi_version, mpi_libversion) == EXIT_SUCCESS)
    {
      selfie_json_double_to_log(out, "mpi_version", (double)mpi_version);
      if (strlen(mpi_libversion) > 0)
      {
	selfie_json_string_to_log(out, "mpi_libversion", mpi_libversion);
      }
    }
  }
  mpi_print = !selfie_getenv("SELFIE_MPI_PRINT");
  output[OUTPUT_ROWMAX - 1] = '\0';
  if (mpi_print != 0)
  {
    snprintf(output, OUTPUT_ROWMAX - 1, "\n=== SELFIE MPI DETAILS ===\n\n");
    selfie_strcat_output_to_params_out(out, output);
    snprintf(output, OUTPUT_ROWMAX - 1, "%20s\t%12s\t%12s\n\n", "Function",
	     "Count", "Time");
    selfie_strcat_output_to_params_out(out, output);
    for (i = 0; i < N_MPI_FUNCTIONS; i++)
    {
      if (selfie_mpi_global_data[i].function_count > 0)
      {
	tmp_string = selfie_get_mpi_function_name(i);
	snprintf(output, OUTPUT_ROWMAX - 1, "%20s\t%12llu\t%10.2lf\n",
		 tmp_string, selfie_mpi_global_data[i].function_count,
		 selfie_mpi_global_data[i].function_time);
	selfie_strcat_output_to_params_out(out, output);
	free(tmp_string);
      }
    }
    snprintf(output, OUTPUT_ROWMAX - 1, "\n");
    selfie_strcat_output_to_params_out(out, output);
  }

  return EXIT_SUCCESS;
};

/// \brief structure for plugin mpi
plugins_functions selfie_plugin_mpi = {
    selfie_plugin_mpi_pre, selfie_plugin_mpi_init, selfie_plugin_mpi_finalize};
}
#endif
