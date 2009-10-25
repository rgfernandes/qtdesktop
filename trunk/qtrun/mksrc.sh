#!/bin/sh
pushd .. >/dev/null
tar zcf ~/RPM/SOURCES/qtrun-`cat qtrun/ver`.tar.gz qtrun
popd >/dev/null
