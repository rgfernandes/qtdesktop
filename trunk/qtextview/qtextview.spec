Name:		qtextview
Version:	0.0.1
Release:	1%{?dist}
License:	GPLv3
Url:		http://code.google.com/p/qtextview/
Source0:	http://qxkb.googlecode.com/files/%{name}-%{version}.tar.xz
Summary:	Qt-based text viewer
BuildRequires:	pkgconfig(QtGui), desktop-file-utils

%description
Just a simple plain text viewer.

%prep
%setup -q

%build
`pkg-config --variable=exec_prefix QtCore`/bin/qmake
make %{?_smp_mflags}

%install
%{makeinstall} INSTALL_ROOT=%{buildroot}
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop

%files
%doc COPYING
%{_bindir}/%{name}
#{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
* Mon Apr 01 2013 TI_Eugene <ti.eugene@gmail.com> 0.0.1-1
- initial packaging for Fedora
