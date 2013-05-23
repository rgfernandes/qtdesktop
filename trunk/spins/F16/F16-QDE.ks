# fedora-livecd-qtdesktop.ks
#
# Description:
# - Fedora Live Spin with the QtDesktop Environment (601 packages; 1.2G, 350M => 210M)
#
# Maintainer(s):
# - TI_Eugene <ti.eugene@gmail.com>
# Usage:
# - sudo livecd-creator -c F16-QDE.ks -t /mnt/shares/tmp --cache /mnt/shares/cache/yum
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

# 1. official
# 1.1. cleanup
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
-btrfs-progs

# 1.2. add (non-Qt)
anaconda
mc
rpmreaper
slim
openbox
oxygen-icon-theme

# 1.3. add (Qt)
AcetoneISO2
arora
avidemux-qt
clementine
#eric
goldendict
kchmviewer-qt
#lyx
#monkeystudio
psi
#q4wine
qbittorrent
#qdevelop
#qgit
qmmp
qmmp-plugins-freeworld
#qstardict
#qt-creator
qutim-icq
qutim-jabber
qutim-mrim
scribus
smplayer
speedcrunch
sqliteman
vlc
#yagf

# 2. qtdesktop (add)
razorqt
razorqt-desktop
razorqt-panel
razorqt-runner
razorqt-config
razorqt-session
andromeda
basqet
communi
cuberok
#cuneiform-qt
flo
juffed-plugins
ptbatterysystemtray
qasmixer
qgmailnotifier
qiviewer
qlipper
qrdc
qtagconvert
qterminal
qtfm
qtpdfview
qxkb
qxmledit
#scantailor
screengrab
socnetv
#ucpufreq
znotes

%end

%post

sed -i -e 's/xfce4,icewm,wmaker,blackbox/razor-openbox,openbox/g' /etc/slim.conf
echo "exec /usr/bin/razor-session -c session-openbox" > ~/.xinitrc
echo "DISPLAYMANAGER=/usr/bin/slim" > /etc/sysconfig/desktop
echo "[Desktop Entry]
Name=Install To Hard Drive
GenericName=Install
Comment=Install the live CD to your hard disk
Categories=System;Utility;X-Red-Hat-Base;X-Fedora;GNOME;GTK;Qt;
Exec=/usr/bin/libeinst
Terminal=false
Type=Application
Icon=anaconda
Encoding=UTF-8
StartupNotify=true
NoDisplay=false
X-Desktop-File-Install-Version=0.18
" > /usr/share/applications/liveinst.desktop

%end
