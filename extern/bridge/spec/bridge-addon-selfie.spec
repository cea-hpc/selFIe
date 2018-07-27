Name:           bridge-addon-selfie
Version:        1.3
Release:        4%{?dist}
Summary:        Bridge addon for enable selfie profiling in Bridge jobs

License:        GPL License
URL:            https://github.com/cea-hpc/bridge
Source:         %{name}-%{version}.tar.gz

Requires:       bridge

%description
Bridge addon for enable selfie profiling in Bridge jobs

%prep
%setup -q -n %{name}-%{version}

%build

%install
# Clean directories
rm -rf $RPM_BUILD_ROOT
# Create directories where to put files
mkdir -p $RPM_BUILD_ROOT/etc/bridge
mkdir -p $RPM_BUILD_ROOT/usr/share/scripts/addons
# Moving files
install -m 644 selfie.ad $RPM_BUILD_ROOT/usr/share/scripts/addons
install -m 644 bridge_selfie.conf $RPM_BUILD_ROOT/etc/bridge

%clean
rm -rf $RPM_BUILD_ROOT
rm -rf %{_tmppath}/%{name}
rm -rf %{_topdir}/BUILD/%{name}

%files
# list files owned by the package
%defattr(-,root,root)
%attr(0644,root,root) /usr/share/scripts/addons/selfie.ad
%attr(0644,root,root) /etc/bridge/bridge_selfie.conf

%changelog
* Tue Jul 26 2018 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.3-4
- Correction in selfie.ad

* Tue Jul 26 2018 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.3-3
- Correction in selfie.ad

* Tue Jul 26 2018 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.3-2
- Change default output file naming policy

* Wed Jul 25 2018 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.3

* Wed May 18 2016 Laurent Nguyen <laurent.nguyen@cea.fr>
- 1.0 First release
