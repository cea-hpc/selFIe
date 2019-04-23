///
/// @file   selfie_omp.h
/// @author COTTE Adrien <adrien@cotte.com>
/// @date   Mon Jul 4 12:00:00 2016
///
/// @brief  Header file for selfie_omp.h
///
///
///
#ifndef _SELFIE_omp_H_
#define _SELFIE_omp_H_
#include "selfie.h"
#include "selfie_tools.h"

/// \brief      preInitialize plugin
/// \return     exit code
int selfie_plugin_omp_pre();

/// \brief      Initialize plugin
/// \param[in]  in  Input parameter for plugin
/// \param[out] out Output parameter for plugin
/// \return     exit code
int selfie_plugin_omp_init(params_in *in, params_out *out);

/// \brief      Finalize plugin
/// \param[in]  in  Input parameter for plugin
/// \param[out] out Output parameter for plugin
/// \return     exit code
int selfie_plugin_omp_finalize(params_in *in, params_out *out);

/// \brief structure for plugin omp
extern plugins_functions selfie_plugin_omp;
#endif
