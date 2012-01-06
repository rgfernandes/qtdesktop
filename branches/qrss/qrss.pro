TEMPLATE = app
QT = gui \
 core \
 xml \
 webkit \
 network \
 svg \
 sql
CONFIG += qt warn_on console debug_and_release
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui
HEADERS = src/mainwindowimpl.h \
 src/httphandler.h \
 src/httpobserver.h \
 src/qrssglobals.h \
 src/rsschannel.h \
 src/rsscontainer.h \
 src/rssitem.h \
 src/rssitemtree.h \
 src/xmlparser.h
SOURCES = src/mainwindowimpl.cpp \
 src/main.cpp \
 src/httphandler.cpp \
 src/rsschannel.cpp \
 src/rsscontainer.cpp \
 src/rssitem.cpp \
 src/rssitemtree.cpp \
 src/xmlparser.cpp
TRANSLATIONS += l10n/qrss_ru.ts
RESOURCES += qrss.qrc
translations.files = l10n/*.qm
desktop.files = qrss.desktop
icon.files = icons/qrss.svg
unix {
 isEmpty(PREFIX) {
  PREFIX =   /usr
 }
 target.path =  $$PREFIX/bin
 translations.path =  $$PREFIX/share/qrss/l10n
 desktop.path =  $$PREFIX/share/applications
 icon.path =  $$PREFIX/share/pixmaps
 INSTALLS +=  target translations  desktop  icon
}
