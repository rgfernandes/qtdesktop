# F15_QDE-nano.ks
# - w/o installer to disk
#

%include F15_QDE-base.ks

%packages

-anaconda
-anaconda-yum-plugins
-audit
-authconfig-gtk
-cairo-gobject
-checkpolicy
-cracklib-python
-createrepo
-cryptsetup-luks
-diffutils
-dosfstools
-fcoe-utils
-firstboot
-GConf2
-gdk-picbuf2
-genisoimage
-gnome-python2
-gnome-python2-canvas
-gtk2
-gtk3
-iptables-ipv6
-iscsi-initiator-utils
-isomd5sum
-jasper-libs
-krb5-workstation
-libart_lgpl
-libcanberra
-libcanberra-gtk3
-libconfig
-libglade2
-libgnomecanvas
-libhbaapi
-libhbalinux
-libIDL
-libnotify
-libselinux-python
-libselinux-utils
-libsemanage
-libuser-python
-libxml2-python
-lldpad
-lvm2
-lvm2-libs
-makebootfat
-mdadm
-mtools
-notification-daemon
-ORBit2
-parted
-policycoreutils
-pycairo
-pygobject2
-pygtk2
-pygtk2-libglade
-pykickstart
-pyparted
-python-cryptsetup
-python-deltarpm
-python-ethtool
-python-meh
-python-nss
-python-pyblock
-python-slip
-report-gtk
-report-newt
-rng-tools
-setuptools
-squashfs-tools
-syslinux
-system-config-date
-system-config-firewall-base
-system-config-keyboard
-system-config-users
-tigervnc-license
-tigervnc-server-minimal
-usermode
-usermode-gtk
-ustr
-xml-common
-zenity

%end