#!/bin/sh
# Make QDE-* package
# $1 - name (gcc, gcc-c++, libpng
# E.g. ./mk_any.sh gcc
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
VER=`rpm -qi mingw32-$1 | grep ^Version | gawk '{print $3}'`
REL=`rpm -qi mingw32-$1 | grep ^Release | gawk '{print $3}'| sed 's/\.fc17//'`
DEST=`pwd`/QDE-$1-$VER-$REL.zip
TMPDIR=`mktemp -d`
pushd $TMPDIR
mkdir lib
# 1. cp files
for i in `rpm -ql mingw32-$1 | grep dll$`
do
    cp $i lib/
done
# 2. prepare [Un]install scripts
TMPINSTALL=`mktemp`
TMPUNINSTALL=`mktemp`
find . -type f | sed 's/^\.\///' | sed 's/\//\\\\/' | while read i
do
    echo "MOVE $i %SystemDrive%\\QtDesktop\\lib\\" >> $TMPINSTALL
    echo "DEL %SystemDrive%\\QtDesktop\\$i" >> $TMPUNINSTALL
done
echo "RMDIR lib" >> $TMPINSTALL
mv $TMPINSTALL Install.bat
mv $TMPUNINSTALL Uninstall.bat
unix2dos Install.bat Uninstall.bat
# 3. pack
zip $DEST *
popd
rm -rf $TMPDIR
