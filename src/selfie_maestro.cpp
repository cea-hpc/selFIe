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
/// @file   selfie_maestro.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Wed Apr 22 12:00:00 2020
///
/// @brief  Main file for plugin selfie_maestro
///
///
///
#include "config.h"
#ifdef HAVE_MAESTRO
extern "C"
{
#include "selfie_tools.h"
#include "selfie_maestro.h"
}
#include <unistd.h>
#include <limits.h>
#include <dlfcn.h>
#include <cstdio>
/// \typedef maestro_plugin_global_data
/// \brief   maestro_plugin_global_data of struct Maestro_plugin_global_data
///
/// \details Structure for output parameter of plugins
///
typedef struct Maestro_plugin_global_data
{
  /// number of MAESTRO function calls
  unsigned long long int function_count;
  /// duration of MAESTRO function calls
  double function_time;
  /// Prefix
  char *jprefix;
} maestro_plugin_global_data;

extern maestro_plugin_global_data selfie_maestro_global_data[];

#include "selfie_maestro_functions.h"

/// \brief maestro plugin data (GLOBAL)
maestro_plugin_global_data selfie_maestro_global_data[N_MAESTRO_FUNCTIONS];

extern "C"
{
  /// \details   preInitialize here all variables for plugin
  int selfie_plugin_maestro_pre()
  {
    int i = 0;
    char *tmp = NULL;
    for (i = 0; i < N_MAESTRO_FUNCTIONS; i++)
    {
      tmp = selfie_get_maestro_function_name(i);
      selfie_maestro_orig_pointer_functions[i] =
	  (function_type)dlsym(RTLD_NEXT, tmp);
      free(tmp);
    }

    return EXIT_SUCCESS;
  };

  /// \details   Initialize here all variables for plugin
  int selfie_plugin_maestro_init(params_in *in, params_out *out)
  {
    int i = 0;
#ifdef HAVE_DEBUG
    PINFO("");
#endif

    for (i = 0; i < N_MAESTRO_FUNCTIONS; i++)
    {
      selfie_maestro_global_data[i].jprefix = out->jprefix;
      selfie_maestro_global_data[i].function_count = (unsigned long long int)0;
      selfie_maestro_global_data[i].function_time = 0.0;
    }
    return EXIT_SUCCESS;
  };

  /// \details   Release here all variables for plugin
  int selfie_plugin_maestro_finalize(params_in *in, params_out *out)
  {
    int i = 0;
    double maestro_wtime = 0.0;
    unsigned long long int maestro_count = (unsigned long long int)0;
#ifdef HAVE_DEBUG
    PINFO("");
#endif

    if (in->enable != 0)
    {
      for (i = 0; i < N_MAESTRO_FUNCTIONS; i++)
      {
	maestro_wtime += selfie_maestro_global_data[i].function_time;
	maestro_count += selfie_maestro_global_data[i].function_count;
      }

      selfie_json_double_to_log(out, "maestro_time", maestro_wtime);
      selfie_json_llu_to_log(out, "maestro_count", maestro_count);
    }

    return EXIT_SUCCESS;
  };

  /// \brief structure for plugin maestro
  plugins_functions selfie_plugin_maestro = {selfie_plugin_maestro_pre,
					 selfie_plugin_maestro_init,
					 selfie_plugin_maestro_finalize};
}
#endif
