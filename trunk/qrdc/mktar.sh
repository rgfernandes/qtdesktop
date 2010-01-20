#!/bin/sh
VER=`cat ver`
DIR="qrdc-$VER"
rm -rf $DIR
mkdir $DIR
cp -a {contrib,doc,i18n,icons,sql,src,ui,qrdc.pro,qrdc.qrc} $DIR
tar zcf qrdc_$VER.tar.gz $DIR
#rm -rf $DIR