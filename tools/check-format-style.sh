#!/bin/env bash
# Copyright (C) 2015-2017 CEA/DAM                                    
# Copyright (C) 2015-2017 Laurent Nguyen <laurent.nguyen@cea.fr>     
#                                                                    
# This software is governed by the CeCILL-C license under French law and
# abiding by the rules of distribution of free software.  You can  use, 
# modify and/ or redistribute the software under the terms of the CeCILL-C
# license as circulated by CEA, CNRS and INRIA at the following URL       
# "http://www.cecill.info".                                               
#
#
# @file   check-format-style.sh
# @author NGUYEN Laurent <laurent.nguyen@cea.fr>
# @date   Wed Apr 19 12:00:00 2017              
#                                               
# @brief  Script to check if source file is correctly written
#

SOURCEFILE=$1

if [[ ! -f "${SOURCEFILE}" ]] 
then
    echo "File doesn't exist ..."
    exit 1
fi

clang-format ${SOURCEFILE} > ${SOURCEFILE}.new

diff ${SOURCEFILE} ${SOURCEFILE}.new > /dev/null

ret=$?

if [[ $ret -eq 0 ]]
then
    echo "OK"
    ret=0
else
    echo "KO"
    ret=1
fi

rm -f ${SOURCEFILE}.new

exit ${ret}

