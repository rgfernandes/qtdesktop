#!/bin/sh
# Make QDE-Qt4 package
MINGW=/usr/i686-w64-mingw32/sys-root/mingw
DEST=`pwd`/QtDesktop-QtWebKit4-4.8.3-1.zip
TMP=`mktemp -d`
pushd $TMP
# 1. cp files
cp $MINGW/bin/QtWebKit4.dll .
# 2. prepare [Un]install scripts
mkdir .Npackd
echo "MOVE QtWebKit4.dll \"%SystemRoot%\\System32\\QtWebKit4.dll\"" > .Npackd/Install.bat
echo "DEL \"%SystemRoot%\\System32\\QtWebKit4.dll\"" > .Npackd/Uninstall.bat
unix2dos .Npackd/*
# 3. pack
zip -r $DEST * .Npackd/*
popd
rm -rf $TMP
sha1sum $DEST
