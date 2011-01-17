# norootforbuild

%define fedora_version 10

%if 0%{?fedora_version} != 0
%define breq qt4-devel
%define qmake /usr/bin/qmake-qt4
%define lrelease /usr/bin/lrelease-qt4
%endif
%if 0%{?suse_version} != 0
%define breq libqt4-devel
%define qmake /usr/bin/qmake
%define lrelease /usr/bin/lrelease
%endif
%if 0%{?mandriva_version} != 0
%define breq libqt4-devel qt4-linguist
%define qmake /usr/lib/qt4/bin/qmake
%define lrelease /usr/lib/qt4/bin/lrelease
%endif

Name:		qtrun
Version:	0.0.1
Release:	1
License:	GPL
Source:		%{name}-%{version}.tar.gz
Group:		System
Summary:	QT-based run something utility.
Vendor:		TI_Eugene <ti.eugene@gmail.com>
BuildRequires:	gcc-c++, make, %{breq}
BuildRoot:	%{_tmppath}/%{name}-%{version}-build
Prefix:		/usr


%description
%{summary}


%prep
%setup -q -n %{name}


%build
%{lrelease} translations/*.ts
%{qmake}
make


%install
%{__rm} -rf %{buildroot}
%{makeinstall} INSTALL_ROOT=%{buildroot}
install -D -m 644 %{name}.desktop $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
install -D -m 644 system-run.png $RPM_BUILD_ROOT%{_datadir}/pixmaps/%{name}.png


%clean
%{__rm} -rf %{buildroot}


%files
%doc AUTHORS COPYING README INSTALL TODO
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/qt4/translations/%{name}_*.qm
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png


%changelog
* Wed Mar 04 2009 TI_Eugene <ti.eugene@gmail.com> 0.0.1
- Initital build in OBS
