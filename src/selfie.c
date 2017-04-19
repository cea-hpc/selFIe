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
/// @file   selfie.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Main file for selfie
///
///
///
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "selfie.h"
#include "selfie_tools.h"
#include "selfie_plugins.h"

/// \fn      __attribute__((constructor)) void selfie_init(void)
/// \brief   First function to be called by program
///
/// \return
///
/// \details
///
__attribute__((constructor)) void selfie_init(void)
{
  int i = 0;
  params_in input;
  params_out output;
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  // preInitialize plugins
  i = 0;
  while (selfie_plugins[i] != NULL)
  {
    (selfie_plugins[i]->pre)();
    i++;
  }

  // Initialize params
  selfie_alloc_params_in(&input);
  selfie_alloc_params_out(&output);

  // Read configuration file
  selfie_read_config_file(&input);

  // Read runtime environment variables
  selfie_read_env_vars(&input);

  // Check exclude commands
  selfie_check_exclude(&input);

  // Initialize plugins
  i = 0;
  while (selfie_plugins[i] != NULL)
  {
    (selfie_plugins[i]->init)(&input, &output);
    i++;
  }

  // fprintf(stderr,"Debug: %s\n",input.cmdline);

  // Release params
  selfie_free_params_in(&input);
  selfie_free_params_out(&output);
}

/// \fn      __attribute__((destructor)) void selfie_finalize(void)
/// \brief   Last function to be called by program
///
/// \return
///
/// \details Get last profiling information and release resources
///
__attribute__((destructor)) void selfie_finalize(void)
{
  int i = 0;
  params_in input;
  params_out output;
  char *current_locale = NULL;
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  // Initialize params
  selfie_alloc_params_in(&input);
  selfie_alloc_params_out(&output);

  // Read configuration file
  selfie_read_config_file(&input);

  // Read runtime environment variables
  selfie_read_env_vars(&input);

  // Get current locale from code
  current_locale = strdup(setlocale(LC_NUMERIC, ""));

  // Set locale to C (portable) for json output
  setlocale(LC_NUMERIC, "C");

  // Finalize plugins
  i = 0;
  while (selfie_plugins[i] != NULL)
  {
    (selfie_plugins[i]->finalize)(&input, &output);
    i++;
  }

  // Write log
  selfie_write_log(&input, &output);

  // Write output
  selfie_write_output(&input, &output);

  // Set locale to original locale
  setlocale(LC_NUMERIC, current_locale);
  free(current_locale);

  // Release params
  selfie_free_params_in(&input);
  selfie_free_params_out(&output);
}
