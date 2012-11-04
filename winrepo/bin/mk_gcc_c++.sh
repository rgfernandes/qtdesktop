#!/bin/sh
# Make QDE-gcc-c++ package
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
DEST=`pwd`/QDE-gcc-c++-4.7.0-2.7z
TMP=`mktemp -d`
pushd $TMP
mkdir lib
for i in `rpm -ql mingw32-gcc-c++ | grep dll$`; do cp $i lib/; done
7z a $DEST lib/*
popd
rm -rf $TMP
