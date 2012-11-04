#!/bin/sh
# Make QDE-gcc package
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
DEST=`pwd`/QDE-gcc-4.7.0-2.7z
TMP=`mktemp -d`
pushd $TMP
mkdir lib
for i in `rpm -ql mingw32-gcc | grep dll$`; do cp $i lib/; done
7z a $DEST lib/*
popd
rm -rf $TMP
