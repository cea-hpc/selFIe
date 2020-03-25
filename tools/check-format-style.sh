#!/usr/bin/env bash
# Copyright (C) 2015-2019 CEA/DAM                                    
# Copyright (C) 2015-2019 Laurent Nguyen <laurent.nguyen@cea.fr>     
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

ret=0

for SOURCEFILE in $*
do
    if [[ ! -f "${SOURCEFILE}" ]] 
    then
	printf " %-69s  \033[33m[SKIPPED]\033[0m\n" ${SOURCEFILE}
    	continue
    fi

    clang-format ${SOURCEFILE} > ${SOURCEFILE}.new

    diff ${SOURCEFILE} ${SOURCEFILE}.new > /dev/null

    lret=$?

    if [[ $lret -eq 0 ]]
    then
	printf " %-69s  \033[32m[PASSED]\033[0m\n" ${SOURCEFILE}
    else
	printf " %-69s  \033[31m[FAILED]\033[0m\n" ${SOURCEFILE}
	lret=1
    fi

    rm -f ${SOURCEFILE}.new
done 
exit ${ret}

