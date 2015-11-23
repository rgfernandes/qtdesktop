# Problem #

  * QFileDialog (QFD) uses hardcoded QFileSystemModel (QFSM) (or native win/mac/kde/gnome file dialog).
  * QFSM uses hardcoded local filesystem.
  * Then we can't use QFD for virtual filesystems (VFS').
  * And can't use QFSM for file manager - for VFS'.

# Solution #

  * Add own QAbstractFileHandlers (QAFH) for handling VFSs'.
  * Use own QFD successor
  * with own QAbstractItemModel (QAIM) or QFSM successor - QVFSModel.
  * as widget.
  * This widget is used by filemanager, archiver, ftp-client etc
  * but with addition respective menu

## ??? ##

  * Central widget - 1 list/table/icons(?)/column (switchable).
  * Left side: tree(?), favorite, bookmarks - switchable on/off.
  * Topbar: actions for central widget: view (list/table/icons), address.
  * below - custom topbar (for filemanager, archiver, ftp)
  * drug'n'drop
  * no tabs

## Inside ##

  * 1 model per panel
  * switch between QFSM/QVFSM.
  * fs watcher/locker.
  * connection manager (e.g. no more then 1 connection per 1 ftp host).

## Questions ##

  * how to switch between models (QFSM/QVFSM) in widget?
  * how to make connection manager?
  * whether it's possible to add VFS' using qt plugins - to use VFS's transparently for any qt app?

# File manager #

is Widget +

  * 1 or 2 panels
  * tabs
  * custom toolbar: current panel on/off

## QtDesktop ##

for every qt-based app:

  * include VFS's includes (with QAFE children) - for using VFS' in QFile, QDir.
  * replace QFileDialog with own FileDialog.