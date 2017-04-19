///
/// @file   selfie_default.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Main file for plugin selfie_default
///
///
///
#include "config.h"
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "selfie_tools.h"
#include "selfie_default.h"

/// \typedef default_plugin_global_data
/// \brief   default_plugin_global_data of struct Default_plugin_global_data
///
/// \details Structure for output parameter of plugins
///
typedef struct Default_plugin_global_data
{
  /// Start time of program
  double start;
} default_plugin_global_data;

/// \brief default plugin data (GLOBAL)
default_plugin_global_data selfie_default_global_data;

/// \brief   Get usage info of program
///
/// \param[out] max_mem_gb Max RSS memory of program
/// \param[out] utime CPU time used
/// \param[out] stime System time used
/// \return  exit code
///
/// \details
///
int selfie_get_rusage(double *max_mem_gb, double *utime, double *stime)
{
  struct rusage usage;
  struct timeval r_utime, r_stime;

  getrusage(RUSAGE_SELF, &usage);

  r_utime = usage.ru_utime;
  r_stime = usage.ru_stime;

  *max_mem_gb = (double)usage.ru_maxrss / 1048576.0;
  *utime = (double)r_utime.tv_sec + (double)r_utime.tv_usec * 1.e-6;
  *stime = (double)r_stime.tv_sec + (double)r_stime.tv_usec * 1.e-6;

  return EXIT_SUCCESS;
};

/// \details   preInitialize here all variables for plugin
int selfie_plugin_default_pre() { return EXIT_SUCCESS; };

/// \details   Initialize here all variables for plugin
int selfie_plugin_default_init(params_in *in, params_out *out)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  selfie_default_global_data.start = selfie_mysecond();
  return EXIT_SUCCESS;
};

/// \details   Release here all variables for plugin
int selfie_plugin_default_finalize(params_in *in, params_out *out)
{
  double maxmem = 0.0;
  double utime = 0.0;
  double stime = 0.0;
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  out->wtime = selfie_mysecond() - selfie_default_global_data.start;
  selfie_get_rusage(&maxmem, &utime, &stime);

  selfie_json_double_to_log(out, "utime", utime);
  selfie_json_double_to_log(out, "stime", stime);
  selfie_json_double_to_log(out, "maxmem", maxmem);

  return EXIT_SUCCESS;
};

/// \brief structure for plugin default
plugins_functions selfie_plugin_default = {selfie_plugin_default_pre,
					   selfie_plugin_default_init,
					   selfie_plugin_default_finalize};
