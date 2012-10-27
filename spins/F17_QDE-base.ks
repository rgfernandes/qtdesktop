# F17_QDE-base.ks
#
# Description:
# - Fedora Live Spin with the QtDesktop Environment (601 packages; 1.2G HDD, 350M => 210M)
#
# Maintainer(s):
# - TI_Eugene <ti.eugene@gmail.com>

%include /usr/share/spin-kickstarts/fedora-live-mini.ks

repo --name=rpmfusion-free --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=free-fedora-$releasever&arch=$basearch
repo --name=rpmfusion-free-updates --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=free-fedora-updates-released-$releasever&arch=$basearch
repo --name=rpmfusion-nonfree --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=nonfree-fedora-$releasever&arch=$basearch
repo --name=rpmfusion-nonfree-updates --mirrorlist=http://mirrors.rpmfusion.org/mirrorlist?repo=nonfree-fedora-updates-released-$releasever&arch=$basearch

selinux --disabled
firewall --disabled
#part / --size 2048 --fstype ext4
services --enabled=NetworkManager,upower --disabled=lvm2-monitor,iptables,ip6tables,network

%packages

# -cleanup

# +misc
mc
rpmreaper
openbox

# qtdesktop

%end

%post

%end
