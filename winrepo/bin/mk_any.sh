#!/bin/sh
# Make QDE-* package
# $1 - name (gcc, gcc-c++, libpng
# E.g. ./mk_any.sh gcc
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
VER=`rpm -qi mingw32-$1 | grep ^Version | gawk '{print $3}'`
REL=`rpm -qi mingw32-$1 | grep ^Release | gawk '{print $3}'| sed 's/\.fc17//'`
DEST=`pwd`/QDE-$1-$VER-$REL.7z
TMP=`mktemp -d`
pushd $TMP
mkdir lib
for i in `rpm -ql mingw32-$1 | grep dll$`; do cp $i lib/; done
7z a $DEST lib/*
popd
rm -rf $TMP
