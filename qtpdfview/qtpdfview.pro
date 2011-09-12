TEMPLATE = app
VERSION = 0.0.1
TARGET = qtpdfview
QT += core gui xml
OBJECTS_DIR = build
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
CONFIG -= debug_and_release build_all warn_on
unix {
 CONFIG +=  link_pkgconfig
 PKGCONFIG +=  poppler-qt4
 isEmpty(PREFIX) {
  PREFIX =   /usr
 }
 target.path =  $$PREFIX/bin
 DATADIR =  $$PREFIX/share
 ICONDIR =  $$PREFIX/share/icons/hicolor
 desktop.files =  contrib/qtpdfview.desktop
 desktop.path =  $$DATADIR/applications
 icon16.files =  contrib/16x16/qtpdfview.png
 icon16.path =  $$ICONDIR/16x16
 icon20.files =  contrib/20x20/qtpdfview.png
 icon20.path =  $$ICONDIR/20x20
 icon22.files =  contrib/22x22/qtpdfview.png
 icon22.path =  $$ICONDIR/22x22
 icon24.files =  contrib/24x24/qtpdfview.png
 icon24.path =  $$ICONDIR/24x24
 icon32.files =  contrib/32x32/qtpdfview.png
 icon32.path =  $$ICONDIR/32x32
 icon36.files =  contrib/36x36/qtpdfview.png
 icon36.path =  $$ICONDIR/36x36
 icon48.files =  contrib/48x48/qtpdfview.png
 icon48.path =  $$ICONDIR/48x48
 icon64.files =  contrib/64x64/qtpdfview.png
 icon64.path =  $$ICONDIR/64x64
 icon96.files =  contrib/96x96/qtpdfview.png
 icon96.path =  $$ICONDIR/96x96
 icon128.files =  contrib/128x128/qtpdfview.png
 icon128.path =  $$ICONDIR/128x128
 icon192.files =  contrib/192x192/qtpdfview.png
 icon192.path =  $$ICONDIR/192x192
}
HEADERS += src/abstractinfodock.h \
 src/documentobserver.h \
 src/embeddedfiles.h \
 src/fonts.h \
 src/info.h \
 src/metadata.h \
 src/navigationtoolbar.h \
 src/optcontent.h \
 src/pageview.h \
 src/permissions.h \
 src/thumbnails.h \
 src/toc.h \
 src/viewer.h
SOURCES += src/abstractinfodock.cpp \
 src/documentobserver.cpp \
 src/embeddedfiles.cpp \
 src/fonts.cpp \
 src/info.cpp \
 src/main_viewer.cpp \
 src/metadata.cpp \
 src/navigationtoolbar.cpp \
 src/optcontent.cpp \
 src/pageview.cpp \
 src/permissions.cpp \
 src/thumbnails.cpp \
 src/toc.cpp \
 src/viewer.cpp
INSTALLS += target \
 desktop \
 icon16 \
 icon20 \
 icon22 \
 icon24 \
 icon32 \
 icon36 \
 icon48 \
 icon64 \
 icon96 \
 icon128 \
 icon192
TRANSLATIONS += l10n/qtpdfview_ru.ts
