HEADERS += connectioncheck.h \
	connectionchecksettings.h \
	eventer.h
SOURCES += connectioncheck.cpp \
	connectionchecksettings.cpp \
	eventer.cpp
INCLUDEPATH += ../../include \
	/usr/include
CONFIG += qt plugin
QT += core gui
TEMPLATE = lib
RESOURCES += connectioncheck.qrc
FORMS += connectionchecksettings.ui
