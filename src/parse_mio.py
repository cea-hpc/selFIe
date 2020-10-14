#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright (C) 2015-2020 CEA/DAM
# Copyright (C) 2015-2020 Laurent Nguyen <laurent.nguyen@cea.fr>
#
# This file is part of selFIe.
#
# This software is governed by the CeCILL-C license under French law and
# abiding by the rules of distribution of free software.  You can  use,
# modify and/ or redistribute the software under the terms of the CeCILL-C
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info".

"""
Script to generate MIO functions
"""

import os
import sys
import docopt

PROGRAM_NAME = os.path.basename(sys.argv[0])
PROGRAM_MAJOR_VERSION = 0
PROGRAM_MINOR_VERSION = 0
PROGRAM_REVISION_VERSION = 1
PROGRAM_VERSION = str(PROGRAM_MAJOR_VERSION) \
                  + "." + str(PROGRAM_MINOR_VERSION) \
                  + "." + str(PROGRAM_REVISION_VERSION)

PROGRAM_HELP = """
{0} is a tool generate selFIe MIO function include file

Usage:
  {0} INPUT
  {0} INPUT -o OUTPUT
  {0} --help
  {0} --version
  {0} --verbose

Options:
  -o --output OUTPUT     output file  
  -h --help              show this help message and exit
  --version              show version and exit
  -v --verbose           print status messages
""".format(PROGRAM_NAME)

def func_list(d):
    s = ""
    for name in d['name']:
        s += '     "' + name + '",\n'
    return s

def def_func_list(plugin_name, d):
    s = ""
    for i, name in enumerate(d['name']):
        if d['type'][i] != "void":
            s += """  /// \\brief {name}
  ///
  /// \\param ...
  /// \\return {rettype}
  ///
  /// \\details
  ///
  {rettype} {name}(...)
  {{
    double f_start = 0.0;
    int ap_except = 0;
    {rettype} val = ({rettype})0;
    char {plugin_name}log[1024];
    int function_number = {fnum};

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_{plugin_name}_pointer_functions[function_number];
    if (selfie_function == NULL)
    {{
      selfie_function = (function_type)dlsym(RTLD_NEXT, "{name}");
      if (selfie_function == NULL)
	return val;
    }}
    selfie_{plugin_name}_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_{plugin_name}_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf({plugin_name}log,
	    "%s \\"timestamp\\": %llu, \\"function\\": \\"%s\\", \\"wtime\\": %0.2f }}",
	    selfie_{plugin_name}_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_{plugin_name}_global_data[function_number].function_time);
    selfie_trace_log({plugin_name}log, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);

    return val;
  }};

""".format(plugin_name=plugin_name, name=name, fnum=i, rettype=d['type'][i])
        else:
            s += """  /// \\brief {name}
  ///
  /// \\param ...
  /// \\return none
  ///
  /// \\details
  ///
  {rettype} {name}(...)
  {{
    double f_start = 0.0;
    int ap_except = 0;
    char {plugin_name}log[1024];
    int function_number = {fnum};

    function_type selfie_function = NULL;
    selfie_function =
	(function_type)selfie_{plugin_name}_pointer_functions[function_number];
    if (selfie_function == NULL)
    {{
      selfie_function = (function_type)dlsym(RTLD_NEXT, "{name}");
      if (selfie_function == NULL)
	return;
    }}
    selfie_{plugin_name}_global_data[function_number].function_count++;
    f_start = selfie_mysecond();

    ap_except = fedisableexcept(FE_INVALID);
    void *ret = __builtin_apply((void (*)(...))selfie_function,
				__builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_{plugin_name}_global_data[function_number].function_time +=
	selfie_mysecond() - f_start;
    sprintf({plugin_name}log,
	    "%s \\"timestamp\\": %llu, \\"function\\": \\"%s\\", \\"wtime\\": %0.2f }}",
	    selfie_{plugin_name}_global_data[function_number].jprefix,
	    (unsigned long long)time(NULL), __func__,
	    selfie_{plugin_name}_global_data[function_number].function_time);
    selfie_trace_log({plugin_name}log, LOG_PID | LOG_NDELAY);

    __builtin_return(ret);
  }};

""".format(plugin_name=plugin_name, name=name, fnum=i, rettype=d['type'][i])

    return s

def header(plugin_name, nfunc, funclist, deffunc):
    head_str = '''// Copyright (C) 2015-2020 CEA/DAM
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
/// @file   selfie_{plugin}_functions.h
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Wed Apr 22 12:00:00 2020
///
/// @brief  Header file for plugin selfie_{plugin}
///
///
///
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <cstring>
#include <cstdio>
#include <execinfo.h>
#include <dlfcn.h>
#include <cstdarg>
#include <fenv.h>
#include <syslog.h>
#include <time.h>

#pragma STDC FENV_ACCESS ON

typedef void (*function_type)(...);

/// \\brief Total number of MPI functions
#define N_{pluginu}_FUNCTIONS {nfunc}

/// \\brief Return a string containing name of functions
/// \\param[in] i Index
/// \\return   Return a string containing name of functions
///
char *selfie_get_{plugin}_function_name(int i)
{{
  char const *{plugin}_functions_name[] = {{
{funclist}     NULL}};
  return strdup({plugin}_functions_name[i]);
}};

/// \\brief Array of pointers of functions
function_type selfie_{plugin}_orig_pointer_functions[N_{pluginu}_FUNCTIONS] = {{NULL}};

/// \\brief Array of pointers of functions
function_type *selfie_{plugin}_pointer_functions = selfie_{plugin}_orig_pointer_functions;

extern "C"
{{
{deffunc}
}}
'''.format(plugin=plugin_name, pluginu=plugin_name.upper(), nfunc=nfunc, funclist=funclist,deffunc=deffunc)
    return head_str

def extract_between(text, sub1, sub2, nth=1):
    """
    extract a substring from text between two given substrings
    sub1 (nth occurrence) and sub2 (nth occurrence)
    arguments are case sensitive
    """
    # prevent sub2 from being ignored if it's not there
    if sub2 not in text.split(sub1, nth)[-1]:
        return None
    return text.split(sub1, nth)[-1].split(sub2, nth)[0]

def read_inputfile(inputfile):
    function_list = []
    with open(inputfile,"r") as fdi:
        for line in fdi:
            if (len(line) > 1 and not line.startswith('#')):
                function_list += [line[:-1]]
    return function_list

def parse_functions(functions_list):
    d={}
    d["name"] = []
    d["type"] = []
    d["args"] = {}
    d["args"]["type"] = []
    d["args"]["name"] = []
    d["args"]["post"] = []
    
    for function in functions_list:
        d["name"] += [function.split()[1].split('(')[0]]
        d["type"] += [function.split()[0]]
        args_list = extract_between(function, '(', ')')
        name_list = []
        type_list = []
        post_list = []
        tmp = ""
        for args in args_list.split(','):
            arg = args.split()
            if(len(arg) > 1):
                tmp_idx = arg[-1].strip('*').find("[")
                if(tmp_idx < 0):
                    tmp_idx = len(arg[-1].strip('*'))
                name_list += [arg[-1].strip('*')[0:tmp_idx]]
                tmp = arg[0]
                if(tmp == "const"):
                    tmp += " " + arg[1]
                for idx in range(0, args.count('*')):
                    tmp += ' *'
                type_list += [tmp]
                if("[" in arg[-1]):
                    post_list += ["[]"]
                else:
                    post_list += [""]
        d["args"]["name"] += [name_list]
        d["args"]["type"] += [type_list]
        d["args"]["post"] += [post_list]
    return d

def main():
    """
    Main program
    """
    arguments = docopt.docopt(PROGRAM_HELP, version=PROGRAM_VERSION)

    d = parse_functions(read_inputfile(arguments['INPUT']))
    out = header("mio", len(d['name']), func_list(d), def_func_list(plugin_name="mio",d=d))

    if(arguments['--output']):
        outfile = arguments['--output']
        with open(outfile,"w") as fd:
            fd.write(out)
        print("File "+outfile+" written")
    else:
        print(out)

    return


if __name__ == '__main__':
    main()
