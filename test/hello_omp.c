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
/// @file   hello.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Tue Apr 07 12:00:00 2020
///
/// @brief  Test for selfie
///
///
///
#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include "omp.h"
#endif

int main(int argc, char *argv[])
{
#ifdef _OPENMP
#pragma omp parallel
  {
#endif
    printf("Hello World!\n");
#ifdef _OPENMP
  }
#endif
  return EXIT_SUCCESS;
}
