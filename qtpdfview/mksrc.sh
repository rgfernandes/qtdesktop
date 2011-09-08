#!/bin/sh
# pack sources into tbz2
NAME="qtpdfview"
VER=`grep ^VERSION qtpdfview.pro | gawk '{print $3}'`
mkdir $NAME-$VER
cp -r {src,l10n,contrib,AUTHORS,COPYING,$NAME.pro} $NAME-$VER
tar jcf $NAME-$VER.tar.bz2 $NAME-$VER && rm -rf $NAME-$VER
