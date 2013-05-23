# fedora-livecd-qtdesktop.ks
#
# Description:
# - Fedora Live Spin with the QtDesktop Environment (601 packages; 1.2G, 350M => 210M)
#
# Maintainer(s):
# - TI_Eugene <ti.eugene@gmail.com>
# Usage:
# - sudo livecd-creator -c F16_QDE-mini.ks -t /mnt/shares/tmp --cache /mnt/shares/cache/yum
# TODO:
# - services

#%include /usr/share/spin-kickstarts/fedora-live-base.ks
#%include /usr/share/spin-kickstarts/fedora-live-minimization.ks
%include /usr/share/spin-kickstarts/fedora-live-mini.ks

repo --name=rpmfusion-free --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=free-fedora-$releasever&arch=$basearch
repo --name=rpmfusion-free-updates --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=free-fedora-updates-released-$releasever&arch=$basearch
repo --name=rpmfusion-nonfree --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=nonfree-fedora-$releasever&arch=$basearch
repo --name=rpmfusion-nonfree-updates --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=nonfree-fedora-updates-released-$releasever&arch=$basearch
repo --name=qtdesktop --baseurl=http://download.opensuse.org/repositories/home:/TI_Eugene:/QtDesktop/Fedora_$releasever/

selinux --disabled
firewall --disabled
#part / --size 2048 --fstype ext4
#services --enabled=NetworkManager,messagebus --disabled=network,sshd
services --enabled=NetworkManager,messagebus --disabled=network,sshd,lvm2-monitor,iptables,ip6tables,avahi,auditd,sendmail,

%packages

# cleanup
-abrt*
-acl
-audit
-avahi*
-bc
-c-ares
-cyrus-sasl-gssapi
-cyrus-sasl-plain
-efibootmgr
-ethtool
#-fedora-gnome-theme
-fedora-icon-theme
-foo2*
-foo2qpdl
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
-jbigkit-libs
-krb5-auth-dialog
-krb5-workstation
-lcms
-libbonobo*
-libcollection
-libdhash
-libgnome*
-libini_config
-libldb
-libnetfilter_conntrack
-libnfnetlink
-libpath_utils
-libref_array
-libtevent
-lohit*
-m17n*
-m4
-mailcap
-man-db
-mcelog
-microcode_ctl
-mtr
-notify-python
-ntsysv
-openssh-server
-PackageKit-gtk-module
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
-powertop
-pyOpenSSL
-python-paste
-python-simplejson
-quota
-selinux-policy*
-sendmail
-setools-console
-setroubleshoot
-smolt*
-sssd*
-system-config*
-tcp_wrappers
-tcpdump
-tmpwatch
-vconfig

# misc
mc
rpmreaper
slim
openbox

# qtdesktop
razorqt
razorqt-desktop
razorqt-panel
razorqt-runner
razorqt-config
razorqt-session

%end

%post

sed -i -e 's/xfce4,icewm,wmaker,blackbox/razor-openbox,openbox/g' /etc/slim.conf
echo "exec /usr/bin/razor-session -c session-openbox" > ~/.xinitrc
echo "DISPLAYMANAGER=/usr/bin/slim" > /etc/sysconfig/desktop

%end