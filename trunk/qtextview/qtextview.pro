QT += core gui
isEqual(QT_MAJOR_VERSION, 5) {
QT += widgets
DEFINES += HAVE_QT5
}

TARGET = qtextview
TEMPLATE = app
CONFIG += debug_and_release

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build

SOURCES += main.cpp mainwindow.cpp

HEADERS  += mainwindow.h exts.h
FORMS    += mainwindow.ui
RESOURCES += qtextview.qrc
LIBS += -lmimetypes-qt4

target.path = /usr/bin
desktop.files += qtextview.desktop
desktop.path += /usr/share/applications
icon.files += qtextview.png
icon.path += /usr/share/pixmaps

INSTALLS += target desktop icon
