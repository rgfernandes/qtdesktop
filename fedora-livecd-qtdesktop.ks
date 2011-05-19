# fedora-livecd-qtdesktop.ks
#
# Description:
# - Fedora Live Spin with the QtDesktop Environment (601 packages; 1.2G, 350M => 210M)
#
# Maintainer(s):
# - TI_Eugene <ti.eugene@gmail.com>
# Usage:
# - sudo livecd-creator -c fedora-livecd-qtdesktop.ks -t /mnt/shares/tmp --cache /mnt/shares/cache/yum
# TODO:
# - services

#%include /usr/share/spin-kickstarts/fedora-live-base.ks
#%include /usr/share/spin-kickstarts/fedora-live-minimization.ks
%include /usr/share/spin-kickstarts/fedora-live-mini.ks

repo --name=qtdesktop --baseurl=http://download.opensuse.org/repositories/home:/TI_Eugene:/QtDesktop/Fedora_14/
repo --name=rpmfusion-free --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=free-fedora-$releasever&arch=$basearch
repo --name=rpmfusion-free-updates --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=free-fedora-updates-released-$releasever&arch=$basearch
repo --name=rpmfusion-nonfree --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=nonfree-fedora-$releasever&arch=$basearch
repo --name=rpmfusion-nonfree-updates --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=nonfree-fedora-updates-released-$releasever&arch=$basearch

selinux --disabled
firewall --disabled
#part / --size 2048 --fstype ext4
#services --enabled=NetworkManager,messagebus --disabled=network,sshd
services --enabled=NetworkManager,messagebus --disabled=network,sshd,lvm2-monitor,iptables,ip6tables,avahi,auditd,sendmail,

%packages

# cleanup
-abrt*
-avahi*
-iptables*
-m17n*
-lohit*
-ibus*
-system-config*
-sendmail

-gnome-packagekit
-sssd*
-selinux-policy*
-setools-console
-man-db
-quota
-policycoreutils*
-openssh-server
-powertop
-setroubleshoot
-mtr
-mailcap
-cyrus-sasl-plain
-gnome-python2*
-libldb
-c-ares
-libini_config
-libdhash
-PackageKit-gtk-module
-gnome-keyring
-libcollection
-notify-python
-libpath_utils
-libref_array
-cyrus-sasl-gssapi
-m4
-libtevent
-iso-codes
-krb5-auth-dialog
-libgnome*
-libbonobo*
-groff
-smolt*
-gnome-vfs2
-fedora-gnome-theme
-plymouth-system-theme
-python-paste
-python-simplejson
-fedora-icon-theme
-plymouth-theme-charge
-gnome-themes
-pyOpenSSL
-plymouth-plugin*
-gtk2-engines
-plymouth-graphics-libs
-tcp_wrappers
-google-droid*

# misc
mc
rpmreaper
slim
openbox

# qt-apps from official repos
psi
arora
smplayer
vlc
qbittorrent
qmmp
speedcrunch
goldendict

# qtdesktop
razorqt
razorqt-session-openbox
razorqt-session-eggwm
eggwm
cuberok
easyPaint
juffed
licq
qasmixer
qefem
qiviewer
qlipper
qterminal
qtcmd
qtrun
qxkb
screengrab
webrender
znotes

%end

%post

sed -i -e 's/xfce4,icewm,wmaker,blackbox/razor-openbox,rezor-eggwm,openbox/g' /etc/slim.conf
echo "exec /usr/bin/razor-session -c session-openbox" > ~/.xinitrc

%end