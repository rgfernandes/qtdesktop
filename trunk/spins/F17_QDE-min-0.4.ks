# F17_QDE-min-0.4.ks
# - sudo livecd-creator -c F17_QDE-min-0.4.ks -f F17_QDE-min-0.4 -t /mnt/shares/tmp --cache /mnt/shares/cache/yum

%include ./F17_QDE-min.ks

repo --name=trunk --baseurl=http://download.opensuse.org/repositories/home:/TI_Eugene:/QtDesktop/Fedora_$releasever/

%packages

razorqt-desktop
razorqt-session
razorqt-panel

%end
