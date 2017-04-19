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
/// @file   selfie_posixio.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Main file for plugin selfie_posixio
///
///
///
extern "C" {
#include "config.h"
#ifdef HAVE_POSIXIO
}

extern "C" {
#include "selfie_tools.h"
#include "selfie_posixio.h"
}
/// \typedef posixio_plugin_global_data
/// \brief   posixio_plugin_global_data of struct Posixio_plugin_global_data
///
/// \details Structure for output parameter of plugins
///
typedef struct Posixio_plugin_global_data
{
  /// number of POSIXIO function calls
  unsigned long long int function_count;
  /// duration of POSIXIO function calls
  double function_time;
  /// average size of POSIXIO function calls
  double function_size;
} posixio_plugin_global_data;

extern posixio_plugin_global_data selfie_posixio_global_data[];

#ifdef __SELFIE_POSIXIO_BUILTIN__
extern "C" {
typedef struct _IO_FILE FILE;
extern FILE *stderr;
extern int snprintf(char *str, size_t size, const char *format, ...);
}
#endif

#include "selfie_posixio_functions.h"

#ifdef __SELFIE_POSIXIO_HOOK__
#include <cstdio>
#endif

/// \brief posixio plugin data (GLOBAL)
posixio_plugin_global_data selfie_posixio_global_data[N_POSIXIO_FUNCTIONS];

extern "C" {
/// \details   preInitialize here all variables for plugin
int selfie_plugin_posixio_pre()
{
  int i = 0;
  for (i = 0; i < N_POSIXIO_FUNCTIONS; i++)
  {
    char *tmp = selfie_get_posixio_function_name(i);
    selfie_orig_pointer_functions[i] = dlsym(RTLD_NEXT, tmp);
    free(tmp);
  }
  return EXIT_SUCCESS;
};

/// \details   Initialize here all variables for plugin
int selfie_plugin_posixio_init(params_in *in, params_out *out)
{
  int i = 0;
  int enable = 1;
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  enable = selfie_getenv("SELFIE_NOPOSIXIO");

  if (in->enable == 0)
    enable = 0;

  for (i = 0; i < N_POSIXIO_FUNCTIONS; i++)
  {
    selfie_posixio_global_data[i].function_count = (unsigned long long int)0;
    selfie_posixio_global_data[i].function_time = 0.0;
    selfie_posixio_global_data[i].function_size = 0.0;
  }
  if (in->enable == 1)
  {
    selfie_pointer_functions = selfie_new_pointer_functions;
  }

  return EXIT_SUCCESS;
};

/// \details   Release here all variables for plugin
int selfie_plugin_posixio_finalize(params_in *in, params_out *out)
{
  int i = 0;
  double posixio_wtime = 0.0;
  unsigned long long int posixio_count = (unsigned long long int)0;
  int posixio_print = 0;
  char *tmp_string = NULL;
  char output[OUTPUT_ROWMAX] = "";
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  // Stop IO profiling
  selfie_pointer_functions = selfie_orig_pointer_functions;

  if (in->enable == 1)
  {
    for (i = 0; i < N_POSIXIO_FUNCTIONS; i++)
    {
      posixio_wtime += selfie_posixio_global_data[i].function_time;
      posixio_count += selfie_posixio_global_data[i].function_count;
    }

    selfie_json_double_to_log(out, "posixio_time", posixio_wtime);
    selfie_json_llu_to_log(out, "posixio_count", posixio_count);
  }

  posixio_print = !selfie_getenv("SELFIE_POSIXIO_PRINT");
  output[OUTPUT_ROWMAX - 1] = '\0';
  if (posixio_print != 0)
  {
    snprintf(output, OUTPUT_ROWMAX - 1, "\n=== SELFIE POSIXIO DETAILS ===\n\n");
    selfie_strcat_output_to_params_out(out, output);
    snprintf(output, OUTPUT_ROWMAX - 1, "%20s\t%12s\t%12s\n\n", "Function",
	     "Count", "Time");
    selfie_strcat_output_to_params_out(out, output);
    for (i = 0; i < N_POSIXIO_FUNCTIONS; i++)
    {
      if (selfie_posixio_global_data[i].function_count > 0)
      {
	tmp_string = selfie_get_posixio_function_name(i);
	snprintf(output, OUTPUT_ROWMAX - 1, "%20s\t%12llu\t%10.2lf\n",
		 tmp_string, selfie_posixio_global_data[i].function_count,
		 selfie_posixio_global_data[i].function_time);
	selfie_strcat_output_to_params_out(out, output);
	free(tmp_string);
      }
    }
    snprintf(output, OUTPUT_ROWMAX - 1, "\n");
    selfie_strcat_output_to_params_out(out, output);
  }

  return EXIT_SUCCESS;
};

/// \brief structure for plugin posixio
plugins_functions selfie_plugin_posixio = {selfie_plugin_posixio_pre,
					   selfie_plugin_posixio_init,
					   selfie_plugin_posixio_finalize};
}
extern "C" {
#endif
}
