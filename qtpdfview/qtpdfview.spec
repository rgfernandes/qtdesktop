# norootforbuild

%define fedora_version 15

%if 0%{?fedora_version}
	%define breq qt4-devel
	%define qmake /usr/bin/qmake-qt4
	%define lrelease /usr/bin/lrelease-qt4
%endif
%if 0%{?suse_version}
	%define breq libqt4-devel
	%define qmake /usr/bin/qmake
	%define lrelease /usr/bin/lrelease
%endif
%if 0%{?mandriva_version}
	%define breq libqt4-devel qt4-linguist
	%define qmake /usr/lib/qt4/bin/qmake
	%define lrelease /usr/lib/qt4/bin/lrelease
%endif

Name:		qtpdfview
Version:	0.0.1
Release:	0
License:	GPL
Source:		%{name}-%{version}.tar.bz2
Group:		Utility
Summary:	QT-based PDF viewer.
Vendor:		TI_Eugene <ti.eugene@gmail.com>
BuildRequires:	gcc-c++, make, poppler-qt-devel, %{breq}
BuildRoot:	%{_tmppath}/%{name}-%{version}-build
Prefix:		/usr

%description
%{summary}

%prep
%setup -q

%build
#%{lrelease} translations/*.ts
%{qmake}
make


%install
%{__rm} -rf %{buildroot}
%{makeinstall} INSTALL_ROOT=%{buildroot}

%clean
%{__rm} -rf %{buildroot}

%files
%doc AUTHORS COPYING
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/%{name}.png

%changelog
* Thu Sep 08 2011 TI_Eugene <ti.eugene@gmail.com> 0.0.1
- Initital build in OBS
