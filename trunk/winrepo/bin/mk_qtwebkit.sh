#!/bin/sh
# Make QDE-Qt4 package
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
DEST=`pwd`/QtDesktop-QtWebKit4-4.8.3-1.zip
TMP=`mktemp -d`
pushd $TMP
# 1. cp files
mkdir lib
cp $MINGW/bin/QtWebKit4.dll lib/
# 2. prepare [Un]install scripts
mkdir .Npackd
echo "MOVE lib\\QtWebKit4.dll \"%QTDESKTOP%\\lib\\QtWebKit4.dll\"" > .Npackd/Install.bat
echo "RMDIR lib" >> .Npackd/Install.bat
echo "DEL \"%QTDESKTOP%\\lib\\QtWebKit4.dll\"" > .Npackd/Uninstall.bat
unix2dos .Npackd/*
# 3. pack
zip -r $DEST lib/* .Npackd/*
popd
rm -rf $TMP
sha1sum $DEST
