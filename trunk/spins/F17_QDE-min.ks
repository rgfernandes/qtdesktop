# F17_QDE-min.ks
#
# Description:
# - Fedora Live Spin with the QtDesktop Environment (? packages)
#
# Maintainer(s):
# - TI_Eugene <ti.eugene@gmail.com>
#
# Usage (~30' @ P4 3GHz):
# - sudo livecd-creator -c F17_QDE-min.ks -f F17_QDE-min -t /mnt/shares/tmp --cache /mnt/shares/cache/yum
#
# Result: ./livecd-F17_QDE-base-<datetime>.iso (384MB)
#
# Requirements:
# - Live: RAM: HDD:
# - Installed: RAM: HDD:
#
# TODO:
# - Anaconda.desktop

%include ./F17_QDE-base.ks

#part / --size 2048 --fstype ext4

%packages

# -cleanup
-abrt*
-acl
-alsa-utils
-audit
-avahi*
-bash-completion
-bc
-bridge-utils
-btrfs-progs
-c-ares
-celt
-cifs-utils
-cyrus-sasl-gssapi
-cyrus-sasl-plain
-dejavu-*-fonts
-dialog
-efibootmgr
-ethtool
-fedora-icon-theme
-foo2*
-foo2qpdl
-glibmm24
-gnome-keyring
-gnome-packagekit
-gnome-python2*
-gnome-themes
-gnome-vfs2
-google-droid*
-groff
-gtk2-engines
-ibus*
-iptables*
-iptstate
-iso-codes
-jack-audio-connection-kit
-jbigkit-libs
-keyutils
-krb5-auth-dialog
-krb5-workstation
-lcms
-libavc1394
-libbonobo*
-libcollection
-libdhash
-libdmx
-libffado
-libfreebob
-libgnome*
-libiec61883
-libini_config
-libldb
-libnetfilter_conntrack
-libnfnetlink
-libpath_utils
-libraw1394
-libref_array
-libsamplerate
-libsigc++20
-libtalloc
-libtevent
-libwbclient
-libxml++
-libXxf86dga
-lohit*
-m17n*
-m4
-mailcap
-man-db
-mcelog
-microcode_ctl
-mtr
-notify-python
-ntfs-3g
-ntsysv
-openssh-server
-PackageKit
-pam_krb5
-pam_pkcs11
-passwdqc
-passwdqc-lib
-pcsc-lite-libs
-plymouth-graphics-libs
-plymouth-plugin*
-plymouth-system-theme
-plymouth-theme-charge
-policycoreutils*
-polkit-qt
-powertop
-preupgrade
-pulseaudio
-pulseaudio-utils
-pyOpenSSL
-python-paste
-python-simplejson
-quota
-rng-tools
-rtkit
-sane-backends
-selinux-policy*
-sendmail
-setools-console
-setroubleshoot
-smolt*
-speex
-spice-vdagent
-sssd*
-system-config*
-tcp_wrappers
-tcpdump
-tmpwatch
-vconfig
-xorg-x11-utils

# +misc:
slim
dbus-x11	# Logout

%end

%post

sed -i -e 's/xfce4,icewm,wmaker,blackbox/razor-openbox,openbox/g' /etc/slim.conf
echo "exec /usr/bin/razor-session -c session-openbox" > ~/.xinitrc
echo "DISPLAYMANAGER=/usr/bin/slim" > /etc/sysconfig/desktop
sed -i -e 's/NoDisplay=true/NoDisplay=false/g' /usr/share/applications/liveinst.desktop

%end
