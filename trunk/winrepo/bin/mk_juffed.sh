#!/bin/sh
# Make QDE-JuffEd package
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
VER=0.9.1137
REL=1
DEST=`pwd`/QDE-JuffEd-$VER-$REL.7z
DEVEL=`pwd`/QDE-JuffEd-devel-$VER-$REL.7z
TMP=`mktemp -d`
SRC=/mnt/ftp/Version/GIT/juffed
pushd $SRC/build
#mkdir build
#mingw32-cmake ..
#mingw32-make
mingw32-make install DESTDIR=$TMP
popd
pushd $TMP$MINGW
mv bin/* .
mv lib/*.dll .
mv share/pixmaps/juffed.png .
mv share/juffed/* .
7z a $DEST *.exe *.dll *.png plugins/*.dll apis hlschemes l10n
7z a $DEVEL include lib/*.a plugins/*.dll.a
popd
#rm -rf $TMP
