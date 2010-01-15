TEMPLATE = app
QT = gui core
CONFIG += qt release warn_on console
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
 ui/vnc.ui \
 ui/rdp.ui \
 ui/ssh.ui \
 ui/telnet.ui \
 ui/nx.ui
HEADERS = src/mainwindowimpl.h
SOURCES = src/mainwindowimpl.cpp src/main.cpp
TRANSLATIONS += i18n/qrdc_ru.ts
RESOURCES += qrdc.qrc
