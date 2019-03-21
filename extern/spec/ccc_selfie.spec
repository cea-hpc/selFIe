# Specfile to generate a custom RPM to install software in another place
# This RPM include a specific version of PAPI
# You have to type QA_RPATHS=$[ 0x0002 ] rpmbuild -ba ccc_selfie.spec to remove error
# concerning RPATHs 

%define  debug_package %{nil}
# Put this package in SOURCES directory
%define  _papi_version 5.7.0

Name:           selfie
Version:        1.0.2
Release:        1%{?dist}
Summary:        SElf and Light proFIling Engine

License:        GPL License
URL:            https://github.com/cea-hpc/selFIe
Source:         %{name}-%{version}.tar.gz
Source1:        papi-%{_papi_version}.tar.gz

BuildRequires:  gcc gcc-c++ gcc-gfortran bash python autoconf automake m4 libtool glibc-headers glibc-devel doxygen libstdc++ libstdc++-devel chrpath binutils libyaml-devel libyaml
Requires:       libstdc++ glibc binutils libyaml
AutoProv:       0
AutoReq:        0

%define  _prefix /opt/%{name}-%{version}
#%define  _sysconfdir %{_prefix}/etc
%define  _sysconfdir /etc/selfie
%define  selfieconf selfie.conf

%description
Light profiling for all Linux commands

%prep
%setup -q -b 0 -T -D
%setup -q -b 1 -T -D

%build
cd %{_builddir}/papi-%{_papi_version}/src
%configure --prefix=%{_prefix} --libdir=%{_libdir} CC=gcc CFLAGS=-fPIC F77=gfortran FFLAGS=-fPIC --with-perf-events --with-components="rapl" --with-shared-lib=yes
make %{?_smp_mflags}
# Be carefull it will install PAPI in the real path for building selfie!
make install
cd %{_builddir}/%{name}-%{version}
%configure --with-papi=%{_prefix} --with-mpi --with-posixio 
make %{?_smp_mflags}
rm -rf %{_prefix}

%install
rm -rf %{buildroot}
cd %{_builddir}/papi-%{_papi_version}/src
make install DESTDIR=%{buildroot}
cd %{_builddir}/%{name}-%{version}
make install DESTDIR=%{buildroot}

%clean
rm -rf %{_buildrootdir}/*

%files
%config(noreplace) %attr(644,root,root) %{_sysconfdir}/selfie.conf
%defattr(-,root,root)
%dir %{_prefix}
%doc %{_prefix}/share/doc/selfie/README.md
%{_prefix}

%changelog
* Tue Mar 21 2019 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0.2 release 
- Correction to get outputfilename
- Update PAPI version to 5.7.0

* Tue Jul 31 2018 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0.2b0 release 2
- Correction to get outputfilename

* Tue Jul 24 2018 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0.2b0 release
- Add output file feature
- New specfile for RHEL7/CentOS7

* Wed May 18 2016 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0 First release 
