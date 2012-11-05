#!/bin/sh
# Make QDE-Qt4 package
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
DEST=`pwd`/QtDesktop-Qt4-4.8.3-1.zip
TMP=`mktemp -d`
pushd $TMP
# 1. cp files
cp $MINGW/bin/Q*.dll .
cp -r $MINGW/lib/qt4/* .
find . -type f -name "*d4.dll" -delete
rm -f QtWebKit4.dll
# 2. prepare [Un]install scripts
TMPINSTALL=`mktemp`
TMPUNINSTALL=`mktemp`
for i in `ls *.dll`
do
    echo "MOVE $i \"%SystemRoot%\\System32\\$i\"" >> $TMPINSTALL
    echo "DEL \"%SystemRoot%\\System32\\$i\"" >> $TMPUNINSTALL
done
echo "REG ADD \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\" /v QTDIR /t REG_SZ /d \"%CD%\" /f" >> $TMPINSTALL
echo "REG DELETE \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\" /v QTDIR /f" >> $TMPUNINSTALL
unix2dos $TMPINSTALL $TMPUNINSTALL
mkdir .Npackd
mv $TMPINSTALL .Npackd/Install.bat
mv $TMPUNINSTALL .Npackd/Uninstall.bat
# 3. pack
zip -r $DEST * .Npackd/*
#7za a $DEST lib/* qt4 .Npackd/*
popd
rm -rf $TMP
sha1sum $DEST
