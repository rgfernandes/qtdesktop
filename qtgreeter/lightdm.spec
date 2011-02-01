# norootforbuild

%define fedora_version 14

%if 0%{?fedora_version}
	%define breq qt4-devel
	%define qmake qmake-qt4
	%define lrelease lrelease-qt4
%endif
%if 0%{?mandriva_version}
	%define breq libqt4-devel, qt4-linguist
	%define qmake qmake
	%define lrelease lrelease
%endif
%if 0%{?suse_version}
	%define breq libqt4-devel
	%define qmake qmake
	%define lrelease lrelease
%endif

Name:		lightdm
Version:	0.2.3
Release:	0
License:	GPL
Source:		%{name}-%{version}.tar.gz
#Patch:		%{name}.diff
Group:		User Interface/X
Summary:	LightDB
URL:		http://people.ubuntu.com/~robert-ancell/lightdm/releases/
BuildRequires:	gcc-c++, intltool, pam-devel, libXdmcp-devel, dbus-glib-devel
BuildRoot:	%{_tmppath}/%{name}-%{version}-build
Requires:	%{name}-greeter
Prefix:		/usr

%package	gobject
Summary:	Gobject library for LightDM
Group:		User Interface/X

%package	gobject-devel
Summary:	Gobject library for LightDM
Group:		Development/Libraries
Requires:	%{name}-gobject

%package	gtk-greeter
Summary:	GTK greeter for LightDM
Group:		User Interface/X
Requires:	%{name}-gobject
Provides:	%{name}-greeter

%package	webkit-greeter
Summary:	WebKit greeter for LightDM
Group:		User Interface/X
BuildRequires:	webkitgtk-devel, libxklavier-devel
Requires:	%{name}-gobject
Provides:	%{name}-greeter

%package	qt
Summary:	Qt library for LightDM
Group:		User Interface/X
BuildRequires:	%{breq}

%package	qt-devel
Summary:	Qt development library for LightDM
Group:		Development/Libraries
Requires:	%{name}-qt

%description
%{summary}

%description	gobject
%{summary} - Gobject API

%description	gobject-devel
%{summary} - gtk development library

%description	gtk-greeter
%{summary} - Gtk greeter

%description	webkit-greeter
%{summary} - WebKit greeter

%description	qt
%{summary} - Qt API

%description	qt-devel
%{summary} - development library

%prep
%setup -q

%build
./configure \
	--prefix=%{prefix} \
	--disable-static \
	--disable-gtk-doc-html \
	--sysconfdir=%{_sysconfdir} \
	--with-dbus-sys=%{_sysconfdir}/dbus-1/system.d \
	--with-log-dir=/var/log/%{name} \
	--with-xauth-dir=/var/run/%{name}/authority
#	--disable-liblightdm-gobject \
make

%install
%{__rm} -rf %{buildroot}
%{makeinstall} DBUS_SYS_DIR=%{buildroot}/%{_sysconfdir}/dbus-1/system.d
#make install DESTDIR=%{buildroot}
# hack
rm -rf %{buildroot}/%{_datadir}/gtk-doc %{buildroot}/%{_libdir}/*.la

%clean
%{__rm} -rf %{buildroot}

%post	gobject
ldconfig

%post	qt
ldconfig

%postun	gobject
ldconfig

%postun	qt
ldconfig

%files
%defattr(-,root,root,-)
%doc AUTHORS COPYING ChangeLog INSTALL NEWS README
%{_bindir}/%{name}
%{_sysconfdir}/%{name}.conf
%{_sysconfdir}/init/%{name}.conf
%{_sysconfdir}/dbus-1/system.d/*
%{_mandir}/man1/%{name}.*

%files	gobject
%defattr(-,root,root,-)
%{_libdir}/lib%{name}-gobject*.so.*

%files	gobject-devel
%defattr(-,root,root,-)
%{_libdir}/lib%{name}-gobject*.so
%{_libdir}/pkgconfig/lib%{name}-gobject*
%{_includedir}/%{name}-gobject*

%files	gtk-greeter
%defattr(-,root,root,-)
%{_libexecdir}/ldm-gtk-greeter
%{_datadir}/%{name}/themes/gnome/

%files	webkit-greeter
%defattr(-,root,root,-)
%{_libexecdir}/ldm-webkit-greeter
%{_datadir}/%{name}/themes/webkit/

%files	qt
%defattr(-,root,root,-)
%{_libdir}/lib%{name}-qt*.so.*

%files	qt-devel
%defattr(-,root,root,-)
%{_libdir}/lib%{name}-qt*.so
%{_libdir}/pkgconfig/lib%{name}-qt*
%{_includedir}/%{name}-qt*

%changelog
* Fri Jan 14 2011 TI_Eugene <ti.eugene@gmail.com> - 0.4.60
- Splitting plugins
