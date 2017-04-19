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
/// @file   selfie_papi.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Main file for plugin selfie_papi
///
///
///

#include "config.h"
#ifdef HAVE_PAPI
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "papi.h"

/// \brief Default configuration file name
#define SELFIE_PAPILIB "libpapi.so"

/// \brief Default general configuration file
#ifndef SELFIE_PAPILIBPATH
#define SELFIE_PAPILIBPATH "/usr/lib/"
#endif

/// \brief Environment variable for configuration path
#define ENV_PAPILIB "SELFIE_PAPILIB"

/// \brief Max length for PAPI lib path
#define MAX_PAPILIBPATH 1024

#include "selfie_tools.h"
#include "selfie_papi.h"

/// \typedef papi_plugin_global_data
/// \brief   papi_plugin_global_data of struct Papi_plugin_global_data
///
/// \details Structure for output parameter of plugins
///
typedef struct Papi_plugin_global_data
{
  /// PAPI event set
  int event_set;
  /// Enable papi
  int enable;
  /// List of PAPI functions pointers
  void *papi_handle;
  int (*PAPI_library_init)(int);
  int (*PAPI_multiplex_init)(void);
  int (*PAPI_create_eventset)(int *);
  int (*PAPI_event_name_to_code)(char *, int *);
  int (*PAPI_add_event)(int, int);
  int (*PAPI_start)(int);
  int (*PAPI_stop)(int, long long *);
  int (*PAPI_cleanup_eventset)(int);
  int (*PAPI_destroy_eventset)(int *);
} papi_plugin_global_data;

papi_plugin_global_data selfie_papi_global_data;

/// \brief Number of events
#define PAPINUMEVENT 3

/// \brief        Get event
///
/// \param[in]    index Index of event
/// \return       string event (has to be freed!)
///
/// \details
///
char *selfie_get_event(int index)
{
  char *list_events[PAPINUMEVENT] = {
      "PAPI_TOT_CYC", "PAPI_TOT_INS",
      "OFFCORE_RESPONSE_0:ANY_DATA:ANY_RESPONSE"};
  return strdup(list_events[index]);
}

/// \brief        Get event name
///
/// \param[in]    index Index of event
/// \return       string event name (has to be freed!)
///
/// \details
///
char *selfie_get_event_name(int index)
{
  char *list_events_name[PAPINUMEVENT] = {"total_cycle", "total_instructions",
					  "llc_traffic"
  };
  return strdup(list_events_name[index]);
}

/// \details   Initialize here all PAPI functions for plugin
int selfie_plugin_papi_init_lib(void)
{
  int flag = 1;
  int dlopen_flag = 0;
  char library_path[MAX_PAPILIBPATH] = "";
  struct stat st;
  char *tmp_string = NULL;

#ifdef HAVE_DEBUG
  PINFO("");
#endif

  selfie_papi_global_data.PAPI_library_init =
      (int (*)(int))dlsym(RTLD_NEXT, "PAPI_library_init");
  if (selfie_papi_global_data.PAPI_library_init != NULL)
  {
    flag = 0;
  }
  else
  {
    // Default library path
    strncpy(library_path, SELFIE_PAPILIBPATH, MAX_PAPILIBPATH);
    strncat(library_path, "/", 1);
    strncat(library_path, SELFIE_PAPILIB,
	    MAX_PAPILIBPATH - strlen(library_path) + 1);
    // Get library from env
    if (getenv(ENV_PAPILIB) != NULL)
    {
      tmp_string = strdup(getenv(ENV_PAPILIB));
      strncpy(library_path, tmp_string, MAX_PAPILIBPATH);
      free(tmp_string);
    }

    if (!stat(library_path, &st))
    {
      selfie_papi_global_data.papi_handle =
	  dlopen(library_path, RTLD_NOW | RTLD_LOCAL);
      if (selfie_papi_global_data.papi_handle != NULL)
      {
	if ((selfie_papi_global_data.PAPI_library_init =
		 (int (*)(int))dlsym(selfie_papi_global_data.papi_handle,
				     "PAPI_library_init")) == NULL)
	{
	  flag = 0;
	};
	if ((selfie_papi_global_data.PAPI_multiplex_init =
		 (int (*)(void))dlsym(selfie_papi_global_data.papi_handle,
				      "PAPI_multiplex_init")) == NULL)
	{
	  flag = 0;
	};
	if ((selfie_papi_global_data.PAPI_create_eventset =
		 (int (*)(int *))dlsym(selfie_papi_global_data.papi_handle,
				       "PAPI_create_eventset")) == NULL)
	{
	  flag = 0;
	};
	if ((selfie_papi_global_data.PAPI_event_name_to_code = (int (*)(
		 char *, int *))dlsym(selfie_papi_global_data.papi_handle,
				      "PAPI_event_name_to_code")) == NULL)
	{
	  flag = 0;
	};
	if ((selfie_papi_global_data.PAPI_add_event =
		 (int (*)(int, int))dlsym(selfie_papi_global_data.papi_handle,
					  "PAPI_add_event")) == NULL)
	{
	  flag = 0;
	};
	if ((selfie_papi_global_data.PAPI_start = (int (*)(int))dlsym(
		 selfie_papi_global_data.papi_handle, "PAPI_start")) == NULL)
	{
	  flag = 0;
	};
	if ((selfie_papi_global_data.PAPI_stop =
		 (int (*)(int, long long *))dlsym(
		     selfie_papi_global_data.papi_handle, "PAPI_stop")) == NULL)
	{
	  flag = 0;
	};
	if ((selfie_papi_global_data.PAPI_cleanup_eventset =
		 (int (*)(int))dlsym(selfie_papi_global_data.papi_handle,
				     "PAPI_cleanup_eventset")) == NULL)
	{
	  flag = 0;
	};
	if ((selfie_papi_global_data.PAPI_destroy_eventset =
		 (int (*)(int *))dlsym(selfie_papi_global_data.papi_handle,
				       "PAPI_destroy_eventset")) == NULL)
	{
	  flag = 0;
	};
	if (flag == 0)
	{
	  dlclose(selfie_papi_global_data.papi_handle);
	}
      }
      else
      {
	flag = 0;
      }
      dlerror();
    }
    else
    {
      flag = 0;
    }
  }

  return flag;
};

/// \details   preInitialize here all variables for plugin
int selfie_plugin_papi_pre() { return EXIT_SUCCESS; };

/// \details   Initialize here all variables for plugin
int selfie_plugin_papi_init(params_in *in, params_out *out)
{
  int retval = 0;
  char *list_event[PAPINUMEVENT];
  int i = 0;
  int event_code = 0;
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  selfie_papi_global_data.event_set = PAPI_NULL;
  selfie_papi_global_data.enable = selfie_getenv("SELFIE_NOPAPI");

  if (in->enable == 0)
    selfie_papi_global_data.enable = 0;

  if (selfie_papi_global_data.enable != 0)
  {
    selfie_papi_global_data.enable = selfie_plugin_papi_init_lib();
  }

  if (selfie_papi_global_data.enable != 0)
  {
    for (i = 0; i < PAPINUMEVENT; i++)
    {
      list_event[i] = selfie_get_event(i);
    }

    // Initialize the PAPI library
    retval = selfie_papi_global_data.PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT)
    {
      selfie_papi_global_data.enable = 0;
      return EXIT_FAILURE;
    }

    // Multiplexing
    retval = selfie_papi_global_data.PAPI_multiplex_init();

    // Create the Event Set
    if (selfie_papi_global_data.PAPI_create_eventset(
	    &(selfie_papi_global_data.event_set)) != PAPI_OK)
    {
      selfie_papi_global_data.enable = 0;
      return EXIT_FAILURE;
    }
    for (i = 0; i < PAPINUMEVENT; i++)
    {
      selfie_papi_global_data.PAPI_event_name_to_code(list_event[i],
						      &event_code);
      selfie_papi_global_data.PAPI_add_event(selfie_papi_global_data.event_set,
					     event_code);
    }

    // Free strings
    for (i = 0; i < PAPINUMEVENT; i++)
    {
      free(list_event[i]);
    }

    // Start counting events in the Event Set
    if (selfie_papi_global_data.PAPI_start(selfie_papi_global_data.event_set) !=
	PAPI_OK)
    {
      selfie_papi_global_data.enable = 0;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
};

/// \details   Release here all variables for plugin
int selfie_plugin_papi_finalize(params_in *in, params_out *out)
{
  int i = 0;
  long_long values[PAPINUMEVENT];
  int papi_print = 0;
  char *tmp_string = NULL;
  char output[OUTPUT_ROWMAX] = "";
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  for (i = 0; i < PAPINUMEVENT; i++)
    values[i] = (long_long)0;

  if (selfie_papi_global_data.enable != 0)
  {
    // Stop the counting of events in the Event Set
    if (selfie_papi_global_data.PAPI_stop(selfie_papi_global_data.event_set,
					  values) != PAPI_OK)
    {
      selfie_papi_global_data.enable = 0;
      return EXIT_FAILURE;
    }

    // Clean PAPI
    selfie_papi_global_data.PAPI_cleanup_eventset(
	selfie_papi_global_data.event_set);
    selfie_papi_global_data.PAPI_destroy_eventset(
	&(selfie_papi_global_data.event_set));
    dlclose(selfie_papi_global_data.papi_handle);
#ifdef HAVE_DEBUG
    // Print
    for (i = 0; i < PAPINUMEVENT; i++)
    {
      char *tmp = selfie_get_event_name(i);
      printf("%s : %lu\n", tmp, values[i]);
      free(tmp);
    }
#endif
  }

  if (selfie_papi_global_data.enable != 0)
  {
    // 0: PAPI_TOT_CYC
    // 1: PAPI_TOT_INS
    // 2: OFFCORE_RESPONSE_0:ANY_DATA:ANY_RESPONSE

    // Average Instructions per cycles = PAPI_TOT_INS / PAPI_TOT_CYC
    if (values[0] > (long_long)0)
    {
      selfie_json_double_to_log(out, "papi_ipc",
				(double)values[1] / (double)values[0]);
    }
    else
    {
      selfie_json_double_to_log(out, "papi_ipc", 0.0);
    }
    // Average Memory bandwidth = OFFCORE_RESPONSE_0:ANY_DATA:ANY_RESPONSE * 64
    //                            / Wall time  
    if (out->wtime > 0.0)
    {
      selfie_json_double_to_log(out, "papi_mem_bw",
				(values[2] * 64.0) / (out->wtime * 1048576.0));
    }
    else
    {
      selfie_json_double_to_log(out, "papi_mem_bw", 0.0);
    }
  }
  papi_print = !selfie_getenv("SELFIE_PAPI_PRINT");
  output[OUTPUT_ROWMAX - 1] = '\0';
  if (papi_print != 0)
  {
    snprintf(output, OUTPUT_ROWMAX - 1, "\n=== SELFIE PAPI DETAILS ===\n\n");
    selfie_strcat_output_to_params_out(out, output);
    for (i = 0; i < PAPINUMEVENT; i++)
    {
      tmp_string = selfie_get_event_name(i);
      snprintf(output, OUTPUT_ROWMAX - 1, "%s :\t%llu\n", tmp_string,
	       values[i]);
      selfie_strcat_output_to_params_out(out, output);
      free(tmp_string);
    }
    snprintf(output, OUTPUT_ROWMAX - 1, "%s :\t%lf\n", "papi_ipc",
	     (double)values[1] / (double)values[0]);
    selfie_strcat_output_to_params_out(out, output);
    snprintf(output, OUTPUT_ROWMAX - 1, "%s :\t%lf\n", "papi_mem_bw",
	     (values[2] * 64.0) / (out->wtime * 1048576.0));
    selfie_strcat_output_to_params_out(out, output);
    snprintf(output, OUTPUT_ROWMAX - 1, "\n");
    selfie_strcat_output_to_params_out(out, output);
  }
  return EXIT_SUCCESS;
};

/// \brief structure for plugin papi
plugins_functions selfie_plugin_papi = {selfie_plugin_papi_pre,
					selfie_plugin_papi_init,
					selfie_plugin_papi_finalize};

#endif
