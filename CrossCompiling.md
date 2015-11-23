Tested on Fedora 17

# Pre #
  1. yum install mingw32-qt mingw32-qt-qmake
  1. goto your sources

# Qmake #
  1. mingw32-qmake-qt4 -spec win32-g++-cross

# Cmake #
  1. mkdir build
  1. cd build
  1. cmake .. -DCMAKE\_INSTALL\_PREFIX=/usr -DCMAKE\_BUILD\_TYPE=release -DCMAKE\_TOOLCHAIN\_FILE=/usr/share/mingw/Toolchain-mingw32.cmake
or
  1. mingw32-cmake ...

Note #1: /usr/share/mingw32/Toolchain-mingw32.cmake is from mingw32-filesystem package

Note #2: windres.exe == /usr/bin/i686-pc-mingw32-windres from mingw32-binutils package

# Post #
  1. copy executable into wine directory (e.g. ~/wine\_c/Temp)
  1. copy needed DLLs from mingw32 root directory (e.g. /usr/i686-pc-mingw32/sys-root/mingw/bin/)
  1. run executable