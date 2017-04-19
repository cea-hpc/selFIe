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
/// @file   selfie_plugins.h
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Header file for selfie.h
///
///
///
#include "config.h"
#ifndef _SELFIE_PLUGINS_H_
#define _SELFIE_PLUGINS_H_
#include "selfie.h"

#ifdef HAVE_DUMMY
extern plugins_functions selfie_plugin_dummy;
#endif
#ifdef HAVE_DEFAULT
extern plugins_functions selfie_plugin_default;
#endif
#ifdef HAVE_POSIXIO
extern plugins_functions selfie_plugin_posixio;
#endif
#ifdef HAVE_PAPI
extern plugins_functions selfie_plugin_papi;
#endif
#ifdef HAVE_MPI
extern plugins_functions selfie_plugin_mpi;
extern plugins_functions selfie_plugin_mpiio;
#endif
// ADD NEW PLUGIN BEFORE THIS LINE

/// \brief Arrays of plugins_functions (GLOBAL)
plugins_functions *selfie_plugins[] = {
#ifdef HAVE_DUMMY
    &selfie_plugin_dummy,
#endif
#ifdef HAVE_DEFAULT
    &selfie_plugin_default,
#endif
#ifdef HAVE_POSIXIO
    &selfie_plugin_posixio,
#endif
#ifdef HAVE_PAPI
    &selfie_plugin_papi,
#endif
#ifdef HAVE_MPI
    &selfie_plugin_mpi,
    &selfie_plugin_mpiio,
#endif
    NULL};

#endif
