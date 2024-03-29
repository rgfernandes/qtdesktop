#!/bin/sh
# Make QDE-* package
# $1 - name (gcc, gcc-c++, zlib, libpng, qscintilla)
# E.g. ./mk_any.sh gcc
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
VER=`rpm -qi mingw32-$1 | grep ^Version | gawk '{print $3}'`
REL=`rpm -qi mingw32-$1 | grep ^Release | gawk '{print $3}'| sed 's/\.fc17//'`
DEST=`pwd`/QtDesktop-$1-$VER-$REL.zip
TMPDIR=`mktemp -d`
pushd $TMPDIR
# 1. cp files
for i in `rpm -ql mingw32-$1 | grep dll$`
do
    cp $i .
done
# 2. prepare [Un]install scripts
TMPINSTALL=`mktemp`
TMPUNINSTALL=`mktemp`
for i in `ls`
do
    echo "MOVE $i \"%SystemRoot%\\System32\\$i\"" >> $TMPINSTALL
    echo "DEL \"%SystemRoot%\\System32\\$i\"" >> $TMPUNINSTALL
done
unix2dos $TMPINSTALL $TMPUNINSTALL
mkdir .Npackd
mv $TMPINSTALL .Npackd/Install.bat
mv $TMPUNINSTALL .Npackd/Uninstall.bat
# 3. pack
zip $DEST * .Npackd/*
popd
rm -rf $TMPDIR
sha1sum $DEST
