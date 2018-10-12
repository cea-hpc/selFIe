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
/// @file   selfie.h
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Header file for selfie.c
///
///
///
#ifndef _SELFIE_H_
#define _SELFIE_H_
#include "config.h"

/// \brief Default configuration file name
#ifndef CONFIGFILENAME
#define CONFIGFILENAME "selfie.conf"
#endif

/// \brief Default general configuration file
#ifndef CONFIGFILEPATH
#define CONFIGFILEPATH "/etc/"
#endif

/// \brief Environment variable for configuration path
#define ENVCONFIGFILE "SELFIE_CONFIGFILE"

/// \brief List of environment variables at runtime
#define ENVVARS_RUNTIME                                                        \
  {                                                                            \
    "SELFIE_LOG", "SELFIE_OUTPUTFILE"                                          \
  }

/// \brief For library compatibility
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>

/// \def     PINFO
/// \brief   Print information message for function
///
/// \details
///
#define PINFO(s) fprintf(stderr, "%s - %s - %s\n", "[selfie]", __func__, s)

/// \typedef params_in
/// \brief   params_in of struct Params_in
///
/// \details Structure for input parameter of plugins
///
typedef struct Params_in
{
  /// Minimum execution time for logging
  double time_tol;
  /// Number of environment variables to get
  int nb_env_vars;
  /// List of environment variables to get
  char **env_vars;
  /// List of values of environement variables to get
  char **env_vars_values;
  /// Number of commands to exclude from profiling
  int nb_exclude_commands;
  /// \brief List of commands to exclude from profiling
  char **exclude_commands;
  /// command line (for exclude commands)
  char *cmdline;
  /// enable profiling
  int enable;
  /// \brief Print log
  int log_print;
  /// \brief Print output file
  char *outputfile;
} params_in;

/// \typedef params_out
/// \brief   params_out of struct Params_out
///
/// \details Structure for output parameter of plugins
///
typedef struct Params_out
{
  /// String for logging
  char *log_string;
  /// Index of last character in log_string
  int log_index;
  /// Size of log_string
  int log_size;
  /// String for output
  char *output_string;
  /// Index of last character in output_string
  int output_index;
  /// Size of output_string
  int output_size;
  /// execution time of program
  double wtime;
} params_out;

/// \typedef plugins_functions
/// \brief   plugins_functions of struct Plugins_functions
///
/// \details Structure for plugins (pointers of function)
///
typedef struct Plugins_functions
{
  /// plugin initialize function
  int (*pre)();
  /// plugin initialize function
  int (*init)(params_in *, params_out *);
  /// plugin finalize function
  int (*finalize)(params_in *, params_out *);
} plugins_functions;

#endif
