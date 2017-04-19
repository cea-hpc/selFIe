///
/// @file   selfie_default.h
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Header file for selfie.h
///
///
///
#ifndef _SELFIE_default_H_
#define _SELFIE_default_H_
#include "config.h"
#include "selfie.h"
#include "selfie_tools.h"

/// \brief      preInitialize plugin
/// \return     exit code
int selfie_plugin_default_pre();

/// \brief      Initialize plugin
/// \param[in]  in  Input parameter for plugin
/// \param[out] out Output parameter for plugin
/// \return     exit code
int selfie_plugin_default_init(params_in *in, params_out *out);

/// \brief      Finalize plugin
/// \param[in]  in  Input parameter for plugin
/// \param[out] out Output parameter for plugin
/// \return     exit code
int selfie_plugin_default_finalize(params_in *in, params_out *out);

/// \brief structure for plugin default
plugins_functions selfie_plugin_default;
#endif
