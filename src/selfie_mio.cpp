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
/// @file   selfie_mio.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Wed Apr 22 12:00:00 2020
///
/// @brief  Main file for plugin selfie_mio
///
///
///
#include "config.h"
#ifdef HAVE_MIO
extern "C"
{
#include "selfie_tools.h"
#include "selfie_mio.h"
}
#include <unistd.h>
#include <limits.h>
#include <dlfcn.h>
#include <cstdio>
/// \typedef mio_plugin_global_data
/// \brief   mio_plugin_global_data of struct Mio_plugin_global_data
///
/// \details Structure for output parameter of plugins
///
typedef struct Mio_plugin_global_data
{
  /// number of MIO function calls
  unsigned long long int function_count;
  /// duration of MIO function calls
  double function_time;
  /// Prefix
  char *jprefix;
} mio_plugin_global_data;

extern mio_plugin_global_data selfie_mio_global_data[];

#include "selfie_mio_functions.h"

/// \brief mio plugin data (GLOBAL)
mio_plugin_global_data selfie_mio_global_data[N_MIO_FUNCTIONS];

extern "C"
{
  /// \details   preInitialize here all variables for plugin
  int selfie_plugin_mio_pre()
  {
    int i = 0;
    char *tmp = NULL;
    for (i = 0; i < N_MIO_FUNCTIONS; i++)
    {
      tmp = selfie_get_mio_function_name(i);
      selfie_mio_orig_pointer_functions[i] =
	  (function_type)dlsym(RTLD_NEXT, tmp);
      free(tmp);
    }

    return EXIT_SUCCESS;
  };

  /// \details   Initialize here all variables for plugin
  int selfie_plugin_mio_init(params_in *in, params_out *out)
  {
    int i = 0;
#ifdef HAVE_DEBUG
    PINFO("");
#endif

    for (i = 0; i < N_MIO_FUNCTIONS; i++)
    {
      selfie_mio_global_data[i].jprefix = out->jprefix;
      selfie_mio_global_data[i].function_count = (unsigned long long int)0;
      selfie_mio_global_data[i].function_time = 0.0;
    }
    return EXIT_SUCCESS;
  };

  /// \details   Release here all variables for plugin
  int selfie_plugin_mio_finalize(params_in *in, params_out *out)
  {
    int i = 0;
    double mio_wtime = 0.0;
    unsigned long long int mio_count = (unsigned long long int)0;
#ifdef HAVE_DEBUG
    PINFO("");
#endif

    if (in->enable != 0)
    {
      for (i = 0; i < N_MIO_FUNCTIONS; i++)
      {
	mio_wtime += selfie_mio_global_data[i].function_time;
	mio_count += selfie_mio_global_data[i].function_count;
      }

      selfie_json_double_to_log(out, "mio_time", mio_wtime);
      selfie_json_llu_to_log(out, "mio_count", mio_count);
    }

    return EXIT_SUCCESS;
  };

  /// \brief structure for plugin mio
  plugins_functions selfie_plugin_mio = {selfie_plugin_mio_pre,
					 selfie_plugin_mio_init,
					 selfie_plugin_mio_finalize};
}
#endif
