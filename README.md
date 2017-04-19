# selFIe                                                                        

**selFIe** (SElf and Light proFIling Engine) is a tool to lightly profile Linux commands without compiling.

selFIe is a dynamic library which can be given to the LD_PRELOAD variable before the execution of the command.

```
$ LD_PRELOAD=selfie.so hostname
myhost
```
It doesn't affect the behaviour of the command and users don't see any changes at execution. At the end of the execution, it puts a line in system logs:
```
selfie[26058]: { "utime": 0.00, "stime": 0.01, "maxmem": 0.00, "posixio_time": 0.00, "posixio_count": 7569, "USER": "user", "wtime": 0.01, "command": "/bin/hostname" }
```
The log line is in JSON format for post-processing (like ELK for example).

selFIe can be compiled to profile other features:
* MPI calls (Only C API)
* [PAPI](http://icl.cs.utk.edu/papi/overview/) hardware counters 

## Installation

### Prerequisites

* C and C++ compiler
* [libYAML](http://pyyaml.org/wiki/LibYAML)
* Python

selFIe has been tested on GNU/Linux only

Optional:
* PAPI library

### Compilation

```
$ ./configure --prefix=<path-to-install> --with-libyaml=<path-to-libyaml> --with-posixio 
$ make                                  
$ make install                          
```                                     
To compile with MPI and PAPI profiling:
```
$ ./configure --prefix=<path-to-install> --with-libyaml=<path-to-libyaml> --with-posixio --with-mpi --with-mpiio --with-papi=<path-to-papi>
$ make                                  
$ make install                          
```                                     

## Profiling
To profile your code with selFIe, you only have to add *selfie.so* to the LD_PRELOAD environment variable.
```
$ export LD_PRELOAD=<path-to-install>/lib/selfie.so
$ hostname
myhost
$ mpirun -n 1 ./hello_world
Hello world!
$ grep selfie /var/log/syslog
selfie[26135]: { "utime": 0.01, "stime": 0.00, "maxmem": 0.00, "posixio_time": 0.00, "posixio_count": 7569, "papi_ipc": 0.50, "papi_mem_bw": 13.62, "mpi_time": 0.00, "mpi_count": 0, "mpiio_time": 0.00, "mpiio_count": 0, "USER": "user", "wtime": 0.01, "command": "/bin/hostname" }
selfie[26040]: { "utime": 0.02, "stime": 0.04, "maxmem": 0.01, "posixio_time": 0.04, "posixio_count": 8383, "papi_ipc": 0.93, "papi_mem_bw": 143.62, "mpi_time": 0.10, "mpi_count": 2, "mpi_version": 3.00, "mpi_libversion": "open mpi", "mpiio_time": 0.00, "mpiio_count": 0, "mpiio_version": 3.00, "mpiio_libversion": "open mpi", "USER": "user", "wtime": 0.10, "command": "./a.out" }
```
## Contributing
## Authors
See the list of [AUTHORS](AUTHORS) who participated in this project.
## Contact

Laurent Nguyen - <laurent.nguyen@cea.fr>

## Website

[CEA-HPC](http://www-hpc.cea.fr/)

## Architecture

![alt tag](share/doc/selfie_architecture.png)

selFIe is based on two features:
* selFIe can execute some code before and after main program
* selFIe uses the LD_PRELOAD environment variable to hook POSIXIO or MPI calls

## License

Copyright 2015-2017 CEA/DAM/DIF<br />
<br />
HP2P is distributed under the CeCILL-C. See the included files <br />
Licence_CeCILL-C_V1-en.txt (English version) and <br />
Licence_CeCILL-C_V1-fr.txt (French version) or visit  <br />
http://www.cecill.info for details.

## Acknowledgments
