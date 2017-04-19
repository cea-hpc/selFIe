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
/// @file   selfie_dummy.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Main file for plugin selfie_dummy
///
///
///
#include "config.h"
#include "selfie_tools.h"
#include "selfie_dummy.h"

/// \details   preInitialize here all variables for plugin
int selfie_plugin_dummy_pre() { return EXIT_SUCCESS; };

/// \details   Initialize here all variables for plugin
int selfie_plugin_dummy_init(params_in *in, params_out *out)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  return EXIT_SUCCESS;
};

/// \details   Release here all variables for plugin
int selfie_plugin_dummy_finalize(params_in *in, params_out *out)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  return EXIT_SUCCESS;
};

/// \brief structure for plugin dummy
plugins_functions selfie_plugin_dummy = {selfie_plugin_dummy_pre,
					 selfie_plugin_dummy_init,
					 selfie_plugin_dummy_finalize};
