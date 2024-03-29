TARGET = qtftp
TEMPLATE = app
QT = gui core network
CONFIG += qt \
 warn_on \
 console \
 debug_and_release \
 qtestlib
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui
HEADERS = src/mainwindowimpl.h src/ftp.h src/ftphandler.h
SOURCES = src/mainwindowimpl.cpp src/main.cpp src/ftp.cpp src/ftphandler.cpp
TRANSLATIONS += l10n/qtftp_ru.ts
target.path = /usr/bin
translations.files += l10n/*.qm
translations.path += $$[QT_INSTALL_TRANSLATIONS] /usr/share/qt4/translations
desktop.files += qtftp.desktop
desktop.path += /usr/share/applications
icon.files += qtftp.png
icon.path += /usr/share/pixmaps
INSTALLS += target translations desktop icon
RESOURCES += qtftp.qrc
