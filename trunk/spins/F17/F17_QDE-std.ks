# F17_QDE-min-0.5.ks (35' @ P4-3.0)
# - sudo livecd-creator -c F17_QDE-std.ks -f F17_QDE-std -t /mnt/shares/tmp --cache /mnt/shares/cache/yum

%include ./F17_QDE-min.ks

repo --name=qtdesktop --baseurl=http://download.opensuse.org/repositories/X11:/QtDesktop/Fedora_$releasever/

%packages

qtdesktop
gstreamer-plugins-ugly

%end
