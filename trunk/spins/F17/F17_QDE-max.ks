# F17_QDE-min.ks
#
# Description:
# - Fedora Live Spin with the QtDesktop Environment (? packages)
#
# Maintainer(s):
# - TI_Eugene <ti.eugene@gmail.com>
#
# Usage (~40' @ P-4 3.GHz):
# - sudo livecd-creator -c F17_QDE-max.ks -f F17_QDE-max -t /mnt/shares/tmp --cache /mnt/shares/cache/yum
#
# Result:
# ./F17_QDE-max.iso
#
# Requirements:
# - Live: RAM: HDD:
# - Installed: RAM: HDD:

%include ./F17_QDE-base.ks

#part / --size 2048 --fstype ext4
services --enabled=NetworkManager --disabled=network

%packages

# cleanup
-abrt*

# misc

# qtdesktop
razorqt

%end

%post

sed -i -e 's/xfce4,icewm,wmaker,blackbox/razor-openbox,openbox/g' /etc/slim.conf
echo "exec /usr/bin/razor-session -c session-openbox" > ~/.xinitrc
echo "DISPLAYMANAGER=/usr/bin/slim" > /etc/sysconfig/desktop

%end
