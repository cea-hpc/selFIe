///
/// @file   selfie_omp.c
/// @author COTTE Adrien <adrien@cotte.com>
/// @date   Mon Jul 4 12:00:00 2016
///
/// @brief  Main file for plugin selfie_omp
///
///
///

/// WARNING : This version doesn't support OMP_NESTED.
#include "config.h"
#ifdef HAVE_OMP

extern "C" {

#include "selfie_tools.h"
#include "selfie_omp.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <execinfo.h>
#include <fenv.h>

  /// we need this function to get the number of threads
  extern int omp_get_num_threads();

  /// \typedef selfie_omp_plugin_global_data
  /// \brief   selfie_omp_plugin_global_data of struct selfie_omp_plugin_global_data
  ///
  /// \details Structure for output parameter of plugins
  ///
  typedef struct selfie_omp_plugin_global_data
  {
    /// useful to catch the ponderate sum of threads usage
    long double ponderated_sum;
    /// useful to count the number of OpenMP parallels
    unsigned long long int cnt_parallel;
    /// total time passed in OpenMP parallels
    long double time_parallel;
    /// current nested level
    unsigned long long int level_parallel;
    /// omp_get_num_threads()
    int num_threads;
    /// catch the start of a parallel
    long double time_parallel_start;
  } selfie_omp_plugin_global_data;

  /// \brief gomp plugin data (GLOBAL)
  selfie_omp_plugin_global_data selfie_gomp_plugin_global_data;
  /// \brief iomp plugin data (GLOBAL)
  selfie_omp_plugin_global_data selfie_iomp_plugin_global_data;
  /// \details   preInitialize here all variables for plugin
  int selfie_plugin_omp_pre()
  {
    return EXIT_SUCCESS;
  };

  /// \details   Initialize here all variables for plugin
  int selfie_plugin_omp_init(params_in *in, params_out *out)
  {
#ifdef HAVE_DEBUG
    PINFO("");
#endif

    selfie_gomp_plugin_global_data.cnt_parallel = 0;
    selfie_gomp_plugin_global_data.time_parallel = 0.;
    selfie_gomp_plugin_global_data.ponderated_sum = 0.;
    selfie_gomp_plugin_global_data.level_parallel = 0;

    selfie_iomp_plugin_global_data.cnt_parallel = 0;
    selfie_iomp_plugin_global_data.time_parallel = 0.;
    selfie_iomp_plugin_global_data.ponderated_sum = 0.;
    selfie_iomp_plugin_global_data.level_parallel = 0;
    return EXIT_SUCCESS;
  };

  /// GNU compiler profiling
  typedef void (*GOMP_parallel_start_f_type) (void (*fn) (void*), void *data, unsigned num_threads);
  typedef void (*GOMP_parallel_end_f_type) (void);

  void GOMP_parallel_start(void (*fn) (void*), void *data, unsigned num_threads)
  {
    GOMP_parallel_start_f_type orig_GOMP_parallel_start = NULL;
    orig_GOMP_parallel_start = (GOMP_parallel_start_f_type)
                              dlsym(RTLD_NEXT, "GOMP_parallel_start");
    orig_GOMP_parallel_start(fn, data, num_threads);

    if (selfie_gomp_plugin_global_data.level_parallel++ == 0) {
      selfie_gomp_plugin_global_data.time_parallel_start = selfie_mysecond();
      selfie_gomp_plugin_global_data.cnt_parallel++;
      selfie_gomp_plugin_global_data.num_threads = omp_get_num_threads();
    }
  }

  void GOMP_parallel_end(void)
  {
    GOMP_parallel_end_f_type orig_GOMP_parallel_end = NULL;
    orig_GOMP_parallel_end = (GOMP_parallel_end_f_type)
                            dlsym(RTLD_NEXT, "GOMP_parallel_end");
    orig_GOMP_parallel_end();

    if (--selfie_gomp_plugin_global_data.level_parallel == 0) {
      double time = selfie_mysecond() - selfie_gomp_plugin_global_data.time_parallel_start;
      selfie_gomp_plugin_global_data.time_parallel += time;
      selfie_gomp_plugin_global_data.ponderated_sum += selfie_gomp_plugin_global_data.num_threads * time;
    }
  }

  /// GOMP 4 is used by GNU 4.9 and newer
#ifndef __NO_GOMP_4__
  void GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads,
      unsigned int flags)
  {
    GOMP_parallel_start(fn, data, num_threads);
    fn(data);
    GOMP_parallel_end();
  }
#endif

  /// Intel compiler profiling
  typedef void * (*function_type)();

  int __kmp_fork_call(...)
  {
    if (selfie_iomp_plugin_global_data.level_parallel++ == 0) {
      selfie_iomp_plugin_global_data.time_parallel_start = selfie_mysecond();
      selfie_iomp_plugin_global_data.cnt_parallel++;
    }

    function_type selfie_function = NULL;

    if (selfie_function == NULL) {
      selfie_function = (function_type) dlsym(RTLD_NEXT, "__kmp_fork_call");
      if (selfie_function == NULL) return 0;
    }

    int ap_except = fedisableexcept(FE_INVALID);
    void* ret = __builtin_apply((void (*)(...))selfie_function,
            __builtin_apply_args(), 1024);

    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);

    if (--selfie_iomp_plugin_global_data.level_parallel == 0) {
      double time = selfie_mysecond() - selfie_iomp_plugin_global_data.time_parallel_start;
      selfie_iomp_plugin_global_data.time_parallel += time;
      selfie_iomp_plugin_global_data.ponderated_sum += omp_get_num_threads() * time;
    }
    __builtin_return(ret);
  }

  /// \details   Release here all variables for plugin
  int selfie_plugin_omp_finalize(params_in *in, params_out *out)
  {
#ifdef HAVE_DEBUG
    PINFO("");
#endif

    int omp_num_parallels = selfie_gomp_plugin_global_data.cnt_parallel + selfie_iomp_plugin_global_data.cnt_parallel;

    if (omp_num_parallels == 0) {
      selfie_json_int_to_log(out, "omp_time", 0);
      selfie_json_int_to_log(out, "omp_num_parallels", 0);
      selfie_json_int_to_log(out, "omp_avg_num_threads", 0);
    } else {
      double omp_ponderated_sum = selfie_gomp_plugin_global_data.ponderated_sum + selfie_iomp_plugin_global_data.ponderated_sum;
      double omp_time = selfie_gomp_plugin_global_data.time_parallel + selfie_iomp_plugin_global_data.time_parallel;
      double omp_avg_num_threads = omp_ponderated_sum / omp_time;

      selfie_json_double_to_log(out, "omp_time", omp_time);
      selfie_json_int_to_log(out, "omp_num_parallels", omp_num_parallels);
      selfie_json_double_to_log(out, "omp_avg_num_threads", omp_avg_num_threads);
    }

    return EXIT_SUCCESS;
  };

  /// \brief structure for plugin omp
  plugins_functions selfie_plugin_omp = {
    selfie_plugin_omp_pre,
    selfie_plugin_omp_init,
    selfie_plugin_omp_finalize
  };
}
#endif /* HAVE_OMP */
