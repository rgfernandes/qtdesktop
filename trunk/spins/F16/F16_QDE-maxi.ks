# F16_QDE-maxi.ks
#
# Description:
# - Fedora Live Spin with the QtDesktop Environment (909 packages; 825MB CD, ...GB HDD, ...MB RAM)
#
# Maintainer(s):
# - TI_Eugene <ti.eugene@gmail.com>
# Usage:
# - sudo livecd-creator -c F16_QDE-maxi.ks -t /mnt/shares/tmp --cache /mnt/shares/cache/yum
# Need:
# - livecd-tools, *-kickstarts (custom-kickstarts l10n-kickstarts rpmfusion-free-remix-kickstarts rpmfusion-nonfree-remix-kickstarts spin-kickstarts fedora-kickstarts)

%include F16_QDE-base.ks

%packages

# qt-apps from official repos
AcetoneISO2
arora
avidemux-qt
clementine
eric
goldendict
kchmviewer-qt
lyx
monkeystudio
psi
q4wine
qbittorrent
qdevelop
qgit
qmmp
qmmp-plugins-freeworld
qstardict
qt-creator
qutim-icq
qutim-jabber
qutim-mrim
scribus
smplayer
speedcrunch
sqliteman
vlc
yagf

# qtdesktop
andromeda
basqet
communi
cuberok
cuneiform-qt
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
scantailor
screengrab
socnetv
#ucpufreq
#vacuum
znotes

%end
