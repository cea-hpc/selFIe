#!/bin/env python
# Copyright (C) 2015-2017 CEA/DAM
# Copyright (C) 2015-2017 Laurent Nguyen <laurent.nguyen@cea.fr>
#
# This file is part of SelFIe.
#
# This software is governed by the CeCILL-C license under French law and
# abiding by the rules of distribution of free software.  You can  use,
# modify and/ or redistribute the software under the terms of the CeCILL-C
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info".

import os
import argparse
import re
import textwrap

default_mpi_function_list = [
    "int MPI_Init(int *argc, char ***argv)",
    "int MPI_Finalize(void)",
    "int MPI_Comm_rank(MPI_Comm comm, int *rank)",
    "int MPI_Comm_size(MPI_Comm comm, int *size)",
    "int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)",
    "int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)"
]

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

def get_args_list(args_name, args_type, args_post):
    d = {}
    d["pargs"] = ""
    d["args"] = ""

    for idy,function_name in enumerate(args_name):
        d["pargs"] += args_type[idy]
        d["pargs"] += " "
        d["pargs"] += args_name[idy]
        d["pargs"] += args_post[idy]
        d["pargs"] += ", "
        d["args"] += args_name[idy]
        d["args"] += ", "
    if(len((d["pargs"])) > 0):
        if(d["pargs"][-2] == ','):
            d["pargs"] = d["pargs"][:-2]
        if(d["args"][-2] == ','):
            d["args"] = d["args"][:-2]

    return d

def get_ret_list(rtype):
    d = {}

    dec_ret_val = ""
    get_ret_val = ""
    ret_ret_val = "return"
    if(rtype != "void"):
        dec_ret_val += rtype + " val = ("+rtype+") 0;"
        get_ret_val += "val = "
        ret_ret_val += " val"
    ret_ret_val += ";"
    
    d["dec"] = dec_ret_val
    d["get"] = get_ret_val
    d["ret"] = ret_ret_val
    return d

def parse_mpi_functions(mpi_functions_list):
    d={}
    d["name"] = []
    d["type"] = []
    d["args"] = {}
    d["args"]["type"] = []
    d["args"]["name"] = []
    d["args"]["post"] = []
    
    for function in mpi_functions_list:
        d["name"] += [function.split()[1].split('(')[0]]
        d["type"] += [function.split()[0]]
        args_list = extract_between(function, '(', ')')
        name_list = []
        type_list = []
        post_list = []
        tmp = ""
        for mpi_args in args_list.split(','):
            mpi_arg = mpi_args.split()
            if(len(mpi_arg) > 1):
                tmp_idx = mpi_arg[-1].strip('*').find("[")
                if(tmp_idx < 0):
                    tmp_idx = len(mpi_arg[-1].strip('*'))
                name_list += [mpi_arg[-1].strip('*')[0:tmp_idx]]
                tmp = mpi_arg[0]
                if(tmp == "const"):
                    tmp += " " + mpi_arg[1]
                for idx in range(0,mpi_args.count('*')):
                    tmp += ' *'
                type_list += [tmp]
                if("[" in mpi_arg[-1]):
                    post_list += ["[]"]
                else:
                    post_list += [""]
        d["args"]["name"] += [name_list]
        d["args"]["type"] += [type_list]
        d["args"]["post"] += [post_list]
    return d

def get_mpi_proto_list(d):
    l = []
    for idx,function in enumerate(d["name"]):
        proto = d["type"][idx]+" "+d["name"][idx]+"("
        for idy,function_name in enumerate(d["args"]["name"][idx]):
            proto += d["args"]["type"][idx][idy]
            proto += " "
            proto += d["args"]["name"][idx][idy]
            proto += d["args"]["post"][idx][idy]
            proto += ", "
        if(proto[-2] == ','):
            proto = proto[:-2]
        proto += ")"
        l += [proto]
    return l

def print_selfie_h_header():
    s  = ""
    s += '''#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <cstring>
#include <execinfo.h>
#include <dlfcn.h>
#include <cstdarg>
#include <fenv.h>

#pragma STDC FENV_ACCESS ON

typedef void (*function_type)(...);

'''
    return s

def print_selfie_h_footer():
    s  = ""
    s += '''
}
'''
    return s


def print_selfie_h_n_mpi(d, plugin_name):
    s  = '''
/// \\brief Total number of {1} functions
#define N_{1}_FUNCTIONS {0}

'''.format(str(len(d["name"])), plugin_name.upper())
    return s

def print_selfie_h_get_name(d,plugin_name):
    
    s  = ""
    s +='''/// \\brief Return a string containing name of functions
/// \\param[in] i Index
/// \\return   Return a string containing name of functions
///
char *selfie_get_{0}_function_name(int i)
{{
   char const *{0}_functions_name[] = {{
'''.format(plugin_name)
    for name in d["name"]:
        s += '''      "{0}",\n'''.format(name)
    for name in d["name"]:
        s += '''      "P{0}",\n'''.format(name)
    s += '''      NULL
   }};
   return strdup({0}_functions_name[i]);
}};
'''.format(plugin_name)
    return s

def print_selfie_h_builtin_function(idx, name, symbol, rtype, plugin_name):
    d_ret = get_ret_list(rtype)
    s  = '''
#ifdef __SELFIE_MPI_BUILTIN__
/// \\brief {1}
///
/// \\param ...
/// \\return {3}
///
/// \details
///

{3} {1}(...)
{{
    double f_start = 0.0;
    function_type selfie_function = NULL;
    int ap_except = 0;

    selfie_function = selfie_{4}_pointer_functions[{0}];

    if(selfie_function == NULL)
    {{
       selfie_function = (function_type) dlsym(RTLD_NEXT,"{2}");
    }}

    selfie_{4}_global_data[{0}].function_count++;
    f_start = selfie_mysecond();
    ap_except = fedisableexcept(FE_INVALID);
    void* ret = __builtin_apply(selfie_function, 
                                      __builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    selfie_{4}_global_data[{0}].function_time += selfie_mysecond() - f_start;
    __builtin_return(ret);

}};
#endif
'''.format(idx, name, symbol, rtype, plugin_name)
    return s

def print_selfie_h_functions(d,plugin_name):
    
    s  = ""
    
    for idx,name in enumerate(d["name"]):
        s += print_selfie_h_builtin_function(idx, name, name, 
                                             d["type"][idx], plugin_name)
        s += print_selfie_h_builtin_function(idx, 
                                             "P"+name, name, d["type"][idx],
                                             plugin_name)
    return s

def print_selfie_h_global_array(d,plugin_name):
    s =  '''
/// \\brief Array of pointers of functions
function_type selfie_{1}_orig_pointer_functions[{0}] = {{NULL}};

/// \\brief Array of pointers of functions
function_type *selfie_{1}_pointer_functions = selfie_{1}_orig_pointer_functions;

'''.format(len(d["name"]),plugin_name)
    return s


def print_selfie_h(d,pname):
    
    s  = ""
    s += print_selfie_h_header()
    s += print_selfie_h_n_mpi(d, pname)
    s += print_selfie_h_get_name(d, pname)
    s += print_selfie_h_global_array(d, pname)
    s += "\nextern \"C\" {\n\n"
    s += print_selfie_h_functions(d, pname)
    s += print_selfie_h_footer()
    return s

def read_inputfile(inputfile):
    function_list = []
    with open(inputfile,"r") as fdi:
        for line in fdi:
            if (len(line) > 1):
                function_list += [line[:-1]]
    return function_list

def main():
    parser = argparse.ArgumentParser(
        description="Generate list of MPI functions")
    parser.add_argument("-p","--proto",action="store_true",
                        default=False,
                        help="Print list of MPI functions prototypes")
    parser.add_argument("-i","--input",action="store",
                        default=None,
                        help="File containing MPI functions list")
    parser.add_argument("-n","--name",action="store",
                        default="mpi",
                        help="Name of plugin")
    parser.add_argument("-o","--output",action="store",
                        default=None,
                        help="File where to print "+
                        "result (If None, print to stdout)")

    args = parser.parse_args()
    print("")
    print(parser.description)
    print("")

    header = True

    # Print proto or not
    if(args.proto == True):
        header = False

    # Input file
    if(args.input != None):
        mpi_function_list = read_inputfile(args.input)
    else:
        mpi_function_list = default_mpi_function_list

    # Output file
    if(args.output != None):
        outfile = args.output
    else:
        outfile = None

    pname = args.name

    # Parse functions
    d = parse_mpi_functions(mpi_function_list)

    # Print prototypes
    if(header == False):
        if(outfile == None):
            for proto_name in get_mpi_proto_list(d):
                print(proto_name)
        else:
            with open(outfile,"w") as fd:
                for proto_name in get_mpi_proto_list(d):
                    fd.write(proto_name)
            print("File "+outfile+" written")
    # Print header
    else:
        if(outfile == None):
            print(print_selfie_h(d,pname))
        else:
            with open(outfile,"w") as fd:
                fd.write(print_selfie_h(d,pname))
            print("File "+outfile+" written")
                    

if __name__ == "__main__": main()


