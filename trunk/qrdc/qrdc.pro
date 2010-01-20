TEMPLATE = app
QT = gui core sql
CONFIG += qt \
 warn_on \
 console \
 debug_and_release \
 build_all
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
 ui/protocol.ui \
 ui/var.ui \
 ui/host.ui \
 ui/connection.ui \
 ui/settings.ui
HEADERS = src/mainwindowimpl.h \
 src/protocolimpl.h \
 src/hostimpl.h \
 src/dialogimpl.h \
 src/varimpl.h \
 src/connectionimpl.h
SOURCES = src/mainwindowimpl.cpp \
 src/main.cpp \
 src/protocolimpl.cpp \
 src/hostimpl.cpp \
 src/dialogimpl.cpp \
 src/varimpl.cpp \
 src/connectionimpl.cpp
TRANSLATIONS += i18n/qrdc_ru.ts
RESOURCES += qrdc.qrc
