#!/bin/sh
pushd .. >/dev/null
tar zcf ~/RPM/SOURCES/qrdc-`cat qtrun/ver`.tar.gz qtrun
popd >/dev/null
