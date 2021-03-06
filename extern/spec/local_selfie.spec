Name:           selfie
Version:        1.0.4
Release:        1%{?dist}
Summary:        SElf and Light proFIling Engine

License:        GPL License
URL:            https://github.com/cea-hpc/selFIe
Source:         %{name}-%{version}.tar.gz

BuildRequires:  gcc gcc-c++ gcc-gfortran bash python3 autoconf automake m4 libtool glibc-headers glibc-devel doxygen libstdc++ libstdc++-devel chrpath binutils papi-devel papi libyaml-devel libyaml
Requires:       libstdc++ glibc binutils papi libyaml
AutoProv:       0
AutoReq:        0

%global _prefix /opt/%{name}-%{version}
%global _sysconfdir %{_sysconfdir}/selfie

%description
Light profiling for all Linux commands

%prep
%setup -q 

%build
%configure --with-papi --with-mpi --with-posixio
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%clean
rm -rf %{_buildrootdir}/*

%files
%defattr(-,root,root,755)
%{_prefix}/lib64/selfie.so
%{_prefix}/lib64/selfie.la
%{_prefix}/share/doc/selfie
%doc %{_prefix}/share/doc/selfie/README.md
%config(noreplace) %attr(644,root,root) %{_sysconfdir}/selfie.conf

%changelog
* Wed Mar 25 2020 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0.4 release
- Update scripts for Python3

* Mon Apr 15 2019 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0.3 release
- Add feature for writing logs in a different files in a directory
- Add feature to get more informations with PAPI
- Correction with -Wall compilation option

* Thu Mar 21 2019 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0.2 release
- Add output file feature
- Update specfile for RHEL7/CentOS7

* Tue Jul 24 2018 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0.2b0 release
- Add output file feature
- New specfile for RHEL7/CentOS7

* Wed May 18 2016 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0 First release
