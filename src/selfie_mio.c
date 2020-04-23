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
#include "selfie_tools.h"
#include "selfie_mio.h"

/// \details   preInitialize here all variables for plugin
int selfie_plugin_mio_pre() { return EXIT_SUCCESS; };

/// \details   Initialize here all variables for plugin
int selfie_plugin_mio_init(params_in *in, params_out *out)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  return EXIT_SUCCESS;
};

/// \details   Release here all variables for plugin
int selfie_plugin_mio_finalize(params_in *in, params_out *out)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  return EXIT_SUCCESS;
};

/// \brief structure for plugin mio
plugins_functions selfie_plugin_mio = {selfie_plugin_mio_pre,
				       selfie_plugin_mio_init,
				       selfie_plugin_mio_finalize};
