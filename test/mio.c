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
/// @file   mio.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Wed Apr 22 12:00:00 2020
///
/// @brief  Test for selfie
///
///
///
#include <stdio.h>
#include <stdlib.h>
#include "selfie_mio_test.h"

int main(int argc, char *argv[])
{
  mio_init("test");
  return EXIT_SUCCESS;
}
