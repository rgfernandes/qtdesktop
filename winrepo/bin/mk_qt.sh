#!/bin/sh
# Make QDE-Qt4 package
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
DEST=`pwd`/QDE-Qt4-4.8.3-1.7z
TMP=`mktemp -d`
pushd $TMP
mkdir lib
cp $MINGW/bin/Q*.dll lib/
cp -r $MINGW/lib/qt4 lib/
find . -type f -name "*d4.dll" -delete
7z a $DEST lib/*
popd
rm -rf $TMP
