#!/bin/sh
# Make QDE-Qt4 package
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
DEST=`pwd`/QtDesktop-Qt4-4.8.3-1.7z
TMP=`mktemp -d`
pushd $TMP
# 1. cp files
mkdir lib
cp $MINGW/bin/Q*.dll lib/
cp -r $MINGW/lib/qt4 .
find . -type f -name "*d4.dll" -delete
# 2. prepare [Un]install scripts
TMPINSTALL=`mktemp`
TMPUNINSTALL=`mktemp`
find lib -type f | sed 's/\//\\\\/' | while read i
do
    echo "MOVE $i \"%QTDESKTOP%\\$i\"" >> $TMPINSTALL
    echo "DEL \"%QTDESKTOP%\\$i\"" >> $TMPUNINSTALL
done
echo "MOVE qt4 \"%QTDESKTOP%\\qt4\"" >> $TMPINSTALL
echo "RMDIR lib" >> $TMPINSTALL
echo "REG ADD \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\" /v QTDIR /t REG_SZ /d \"%QTDESKTOP%\\qt4\" /f" >> $TMPINSTALL
echo "RMDIR /S /Q \"%QTDESKTOP%\\qt4\"" >> $TMPUNINSTALL
echo "REG DELETE \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\" /v QTDIR /f" >> $TMPUNINSTALL
unix2dos $TMPINSTALL $TMPUNINSTALL
mkdir .Npackd
mv $TMPINSTALL .Npackd/Install.bat
mv $TMPUNINSTALL .Npackd/Uninstall.bat
# 3. pack
#zip -9 -r $DEST lib/* qt4 .Npackd/*
7za a $DEST lib/* qt4 .Npackd/*
popd
rm -rf $TMP
sha1sum $DEST
