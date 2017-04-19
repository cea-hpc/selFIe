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
/// @file   selfie_tools.h
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Header file for selfie.h
///
///
///
#ifndef _SELFIE_TOOLS_H_
#define _SELFIE_TOOLS_H_
#include "config.h"
#include "selfie.h"

/// \brief Size block allocated block
#define STRING_BLOCK 1024

/// \brief Max length for filename
#define MAX_FILENAME 1024

/// \brief Max length a row in output
#define OUTPUT_ROWMAX 1024

/// \brief       Get time
///
/// \return      get time
///
double selfie_mysecond(void);

/// \brief       Get command line
///
/// \return      exit code
///
int selfie_getcmdline(char *);

/// \brief       Allocate params_in
///
/// \param[out]  in
/// \return      exit code
///
int selfie_alloc_params_in(params_in *in);

/// \brief       Free params_in
///
/// \param[out]  in
/// \return      exit code
///
int selfie_free_params_in(params_in *in);

/// \brief       Allocate params_out
///
/// \param[out]  out
/// \return      exit code
///
int selfie_alloc_params_out(params_out *out);

/// \brief       Free params_out
///
/// \param[out]  out
/// \return      exit code
///
int selfie_free_params_out(params_out *out);

/// \brief       Add string to log string
///
/// \param[out]  out
/// \param[out]  s string to add into log
/// \return      exit code
///
int selfie_strcat_log_to_params_out(params_out *out, char *s);

/// \brief       Add string to output string
///
/// \param[out]  out
/// \param[in]   s string to add into output
/// \return      exit code
///
int selfie_strcat_output_to_params_out(params_out *out, char *s);

/// \brief       Add int value json to log string
///
/// \param[out]  out
/// \param[in]   key json key string
/// \param[in]   value json value integer
/// \return      exit code
///
int selfie_json_int_to_log(params_out *out, char const *key, int value);

/// \brief       Add unsigned long long int value json to log string
///
/// \param[out]  out
/// \param[in]   key json key string
/// \param[in]   value json value integer
/// \return      exit code
///
int selfie_json_llu_to_log(params_out *out, char const *key,
			   unsigned long long int value);

/// \brief       Add double value json to log string
///
/// \param[out]  out
/// \param[in]   key json key string
/// \param[in]   value json value double
/// \return      exit code
///
int selfie_json_double_to_log(params_out *out, char const *key, double value);

/// \brief       Add string value json to log string
///
/// \param[out]  out
/// \param[in]   key json key string
/// \param[in]   value json value string
/// \return      exit code
///
int selfie_json_string_to_log(params_out *out, char const *key,
			      char const *value);

/// \brief       Read configuration file
///
/// \param[out]  in
/// \return      exit code
///
int selfie_read_config_file(params_in *in);

/// \brief       Read environment variables
///
/// \param[out]  in
/// \return      exit code
///
int selfie_read_env_vars(params_in *in);

/// \brief       Get environment variables for enabling plugins
///
/// \param[in]   env
/// \return      flag for enabling
///
int selfie_getenv(char const *env);

/// \brief       Check if cmdline is in exclude commands
///
/// \param[in]   env
/// \return      flag for true of false
///
int selfie_check_exclude(params_in *in);

/// \brief       Write log
///
/// \param[in]   in
/// \param[in]   out
/// \return      exit code
///
int selfie_write_log(params_in *in, params_out *out);

/// \brief       Write output
///
/// \param[in]   in
/// \param[in]   out
/// \return      exit code
///
int selfie_write_output(params_in *in, params_out *out);

#endif
