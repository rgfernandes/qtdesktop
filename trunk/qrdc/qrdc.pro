TEMPLATE = app
QT = gui core sql
CONFIG += qt warn_on console debug
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
 ui/protocol.ui \
 ui/var.ui \
 ui/host.ui \
 ui/connection.ui
HEADERS = src/mainwindowimpl.h
SOURCES = src/mainwindowimpl.cpp src/main.cpp
TRANSLATIONS += i18n/qrdc_ru.ts
RESOURCES += qrdc.qrc
