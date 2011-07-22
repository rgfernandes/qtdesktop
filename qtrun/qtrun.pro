TEMPLATE = app
TARGET = qtrun
target.path = /usr/bin
DEPENDPATH += . translations
INCLUDEPATH += src
HEADERS += src/qtrun.h
SOURCES += src/main.cpp src/qtrun.cpp
TRANSLATIONS += translations/qtrun_ru.ts
trans.path = $$[QT_INSTALL_TRANSLATIONS]
trans.files = translations/qtrun_*.qm
isEmpty(trans.path) {
	trans.path = $(QTDIR)/translations
}
desktop.path = /usr/share/applications
desktop.files = qtrun.desktop
icon.path = /usr/share/pixmaps
icon.files = qtrun.png
QT += core gui
INSTALLS += target trans desktop icon
CONFIG += warn_on
VERSION = 0.0.1
