#!/bin/env python
#
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


default_posixio_list = [
    "FILE* fopen(const char *path, const char *mode)",
    "int fclose(FILE *fp)",
    "int fflush(FILE *stream)",
    "size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)",
    "size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)",
    "int fseek(FILE *stream, long offset, int whence)",
    "long ftell(FILE *stream)",
    "int fgetc(FILE *stream)",
    "int ftruncate(int fd, off_t length)",
    "int ftruncate64(int fd, off64_t length)",
    "int fsync(int fd)"
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

def parse_posixio_functions(l):
    d={}
    d["name"] = []
    d["type"] = []
    d["args"] = {}
    d["args"]["type"] = []
    d["args"]["name"] = []
    d["args"]["post"] = []
    
    posixio_functions_list = l

    for function in posixio_functions_list:
        d["name"] += [function.split()[1].split('(')[0]]
        d["type"] += [function.split()[0]]
        args_list = extract_between(function, '(', ')')
        name_list = []
        type_list = []
        post_list = []
        tmp = ""
        for posixio_args in args_list.split(','):
            posixio_arg = posixio_args.split()
            if(len(posixio_arg) > 1):
                tmp_idx = posixio_arg[-1].strip('*').find("[")
                if(tmp_idx < 0):
                    tmp_idx = len(posixio_arg[-1].strip('*'))
                name_list += [posixio_arg[-1].strip('*')[0:tmp_idx]]
                tmp =""
                for g in posixio_arg[0:-1]:
                    tmp += " " + g
                for idx in range(0,posixio_arg[-1].count('*')):
                    tmp += ' *'
                type_list += [tmp]
                if("[" in posixio_arg[-1]):
                    post_list += ["[]"]
                else:
                    post_list += [""]
        d["args"]["name"] += [name_list]
        d["args"]["type"] += [type_list]
        d["args"]["post"] += [post_list]
    return d

def get_posixio_proto_list(d):
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
#ifdef __SELFIE_POSIXIO_HOOK__
#include <cstdio>
#endif
#ifdef __SELFIE_POSIXIO_BUILTIN__
#include <fenv.h>

#pragma STDC FENV_ACCESS ON

typedef void * (*function_type)();
#endif

'''
    return s

def print_selfie_h_footer():
    s = '''
}
'''
    return s


def print_selfie_h_n(d,plugin_name):
    s = '''
/// \\brief Total number of MPI functions
#define N_{0}_FUNCTIONS {1}
'''.format(plugin_name,len(d["name"]))
    return s

def print_selfie_h_get_name(d,plugin_name):
    s = '''
/// \\brief Return a string containing name of functions
/// \\param[in] i Index
/// \\return   Return a string containing name of functions
///
char *selfie_get_{0}_function_name(int i)
{{
    char const *{0}_functions_name[] = {{
'''.format(plugin_name)
    for name in d["name"]:
        s += '''      "{0}",\n'''.format(name)
    s += '''      NULL
    }};
    return strdup({0}_functions_name[i]);
}};
'''.format(plugin_name)
    return s

def print_selfie_h_ap_pointers_functions(d,plugin_name):
    s = '''
/// \\brief custom functions
void *selfie_new_pointer_functions[] = {
'''
    for name in d["name"]:
        s += '''      (void *) selfie_{0},\n'''.format(name)
    s += '''      NULL
};
'''
    return s

def print_selfie_h_typedef(d):
    s  = '''
#ifdef __SELFIE_POSIXIO_HOOK__
'''
    for idx,name in enumerate(d["name"]):
        d_args = get_args_list(d["args"]["name"][idx], 
                               d["args"]["type"][idx], 
                               d["args"]["post"][idx]
                               )
        s += '''typedef {0} (*orig_{1}_f_type)({2});
'''.format(d["type"][idx],name,d_args["pargs"])
    s += '''#endif
'''
    return s

def print_selfie_h_function_builtin(idx, 
                                       name, symbol, rtype, 
                                       args_name,
                                       args_type,
                                       args_post,
                                       plugin_name):
    d_args = get_args_list(args_name, args_type, args_post)
    d_ret = get_ret_list(rtype)
    s = '''
#ifdef __SELFIE_POSIXIO_BUILTIN__
/// \\brief {0}
///
/// \\param ...
/// \\return {1}
///
/// \\details
///
{1} {0}(...)
{{
    double f_start = 0.0;
    int ap_except = 0;
    {3}

    function_type selfie_function = NULL;

    selfie_function = (function_type) selfie_pointer_functions[{4}];
    
    if(selfie_function == NULL)
    {{
       selfie_function = (function_type) dlsym(RTLD_NEXT,"{0}");
       if(selfie_function == NULL) {8}
    }}

    ap_except = fedisableexcept(FE_INVALID);
    void* ret = __builtin_apply((void (*)(...))selfie_function, 
                                      __builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    __builtin_return(ret);
}};
#endif
'''.format(name,
           rtype,
           d_args["pargs"],
           d_ret["dec"],
           idx,
           plugin_name,
           d_ret["get"],
           d_args["args"],
           d_ret["ret"])
    return s

def print_selfie_h_ap_function_builtin(idx, 
                                       name, symbol, rtype, 
                                       args_name,
                                       args_type,
                                       args_post,
                                       plugin_name):
    d_args = get_args_list(args_name, args_type, args_post)
    d_ret = get_ret_list(rtype)
    s = '''
#ifdef __SELFIE_POSIXIO_BUILTIN__
/// \\brief selfie_{0}
///
/// \\param ...
/// \\return {1}
///
/// \details
///
{1} selfie_{0}(...)
{{
    double f_start = 0.0;
    int ap_except = 0;
    {3} 

    function_type selfie_function = NULL;
    selfie_function = (function_type) selfie_orig_pointer_functions[{4}];

    selfie_{5}_global_data[{4}].function_count++;
    f_start = selfie_mysecond();
    ap_except = fedisableexcept(FE_INVALID);
    void* ret = __builtin_apply((void (*)(...))selfie_function, 
                                      __builtin_apply_args(), 1024);
    feclearexcept(FE_INVALID);
    feenableexcept(ap_except);
    __builtin_return(ret);
    selfie_{5}_global_data[{4}].function_time += selfie_mysecond() - f_start;
   
    {8}
}};
#endif
'''.format(name,
           rtype,
           d_args["pargs"],
           d_ret["dec"],
           idx,
           plugin_name,
           d_ret["get"],
           d_args["args"],
           d_ret["ret"])
    return s

def print_selfie_h_function_hook(idx, 
                                       name, symbol, rtype, 
                                       args_name,
                                       args_type,
                                       args_post,
                                       plugin_name):
    d_args = get_args_list(args_name, args_type, args_post)
    d_ret = get_ret_list(rtype)
    s = '''
#ifdef __SELFIE_POSIXIO_HOOK__
/// \\brief {0}
///
/// \\param ...
/// \\return {1}
///
/// \\details
///
{1} {0}({2})
{{
    double f_start = 0.0;
    {3}

    orig_{0}_f_type selfie_function = NULL;

    selfie_function = (orig_{0}_f_type) selfie_pointer_functions[{4}];
    
    if(selfie_function == NULL)
    {{
       selfie_function = (orig_{0}_f_type) dlsym(RTLD_NEXT,"{0}");
       if(selfie_function == NULL) {8}
    }}
    {6}selfie_function({7});
    {8}
}};
#endif
'''.format(name,
           rtype,
           d_args["pargs"],
           d_ret["dec"],
           idx,
           plugin_name,
           d_ret["get"],
           d_args["args"],
           d_ret["ret"])
   
    return s

def print_selfie_h_ap_function_hook(idx, 
                                       name, symbol, rtype, 
                                       args_name,
                                       args_type,
                                       args_post,
                                       plugin_name):
    d_args = get_args_list(args_name, args_type, args_post)
    d_ret = get_ret_list(rtype)
    s = '''
#ifdef __SELFIE_POSIXIO_HOOK__
/// \\brief selfie_{0}
///
/// \\param ...
/// \\return {1}
///
/// \details
///
{1} selfie_{0}({2})
{{
    double f_start = 0.0;
    {3} 

    orig_{0}_f_type selfie_function = NULL;
    selfie_function = (orig_{0}_f_type) selfie_orig_pointer_functions[{4}];

    selfie_{5}_global_data[{4}].function_count++;
    f_start = selfie_mysecond();
    {6}selfie_function({7});
    selfie_{5}_global_data[{4}].function_time += selfie_mysecond() - f_start;
   
    {8}
}};
#endif
'''.format(name,
           rtype,
           d_args["pargs"],
           d_ret["dec"],
           idx,
           plugin_name,
           d_ret["get"],
           d_args["args"],
           d_ret["ret"])
    return s

def print_selfie_h_functions(d,plugin_name):
    
    s  = ""
    
    for idx,name in enumerate(d["name"]):
        if(not(("f2c" in name) or ("c2f" in name))):
            s += print_selfie_h_function_builtin(idx, name, name, 
                                                    d["type"][idx],
                                                    d["args"]["name"][idx],
                                                    d["args"]["type"][idx],
                                                    d["args"]["post"][idx],
                                                    plugin_name)
            s += print_selfie_h_ap_function_builtin(idx, name, name, 
                                                    d["type"][idx],
                                                    d["args"]["name"][idx],
                                                    d["args"]["type"][idx],
                                                    d["args"]["post"][idx],
                                                    plugin_name)
            s += print_selfie_h_function_hook(idx, name, name, 
                                                    d["type"][idx],
                                                    d["args"]["name"][idx],
                                                    d["args"]["type"][idx],
                                                    d["args"]["post"][idx],
                                                    plugin_name)
            s += print_selfie_h_ap_function_hook(idx, name, name, 
                                                    d["type"][idx],
                                                    d["args"]["name"][idx],
                                                    d["args"]["type"][idx],
                                                    d["args"]["post"][idx],
                                                    plugin_name)

    return s

def print_selfie_h_global_array(d,plugin_name):
    s =  '''
/// \\brief Array of pointers of functions
void *selfie_orig_pointer_functions[{0}] = {{NULL}};

/// \\brief Array of pointers of functions
void **selfie_pointer_functions = selfie_orig_pointer_functions;

'''.format(len(d["name"]))
    return s

def print_selfie_h(d):
    
    s  = ""
    s += print_selfie_h_header()
    s += print_selfie_h_n(d, "POSIXIO")
    s += print_selfie_h_get_name(d,"posixio")
    s += print_selfie_h_global_array(d,"posixio")
    s += print_selfie_h_typedef(d)
    s += "\nextern \"C\" {\n\n"
    s += print_selfie_h_functions(d,"posixio")
    s += print_selfie_h_ap_pointers_functions(d,"posixio")
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
        description="Generate POSIX IO header for selfie")
    parser.add_argument("-p","--proto",action="store_true",
                        default=False,
                        help="Print list of POSIXIO functions prototypes")
    parser.add_argument("-i","--input",action="store",
                        default=None,
                        help="File containing POSIXIO function list")
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
        posixio_list = read_inputfile(args.input)
    else:
        posixio_list = default_posixio_list
        
    # Output file
    if(args.output != None):
        outfile = args.output
    else:
        outfile = None

    # Parse functions
    d = {}
    d = parse_posixio_functions(posixio_list)

    # Print prototypes
    if(header == False):
        if(outfile == None):
            for proto_name in get_posixio_proto_list(d):
                print(proto_name)
        else:
            with open(outfile,"w") as fd:
                for proto_name in get_posixio_proto_list(d):
                    fd.write(proto_name)
            print("File "+outfile+" written")
    # Print header
    else:
        if(outfile == None):
            print(print_selfie_h(d))
        else:
            with open(outfile,"w") as fd:
                fd.write(print_selfie_h(d))
            print("File "+outfile+" written")
                    

if __name__ == "__main__": main()

