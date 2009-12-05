# -------------------------------------------------
# Project created by QtCreator 2009-09-20T17:20:55
# -------------------------------------------------
TARGET = znotes
VERSION = 0.3.7
QT += core \
    gui
TEMPLATE = app
OBJECTS_DIR = build
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
SOURCES += main.cpp \
    mainwindow.cpp \
    configdialog.cpp \
    settings.cpp \
    note.cpp \
    scriptmodel.cpp \
    aboutDialog.cpp \
    toolbarmodel.cpp
HEADERS += mainwindow.h \
    configdialog.h \
    settings.h \
    note.h \
    scriptmodel.h \
    aboutDialog.h \
    toolbaraction.h \
    toolbarmodel.h
FORMS += mainwindow.ui \
    configdialog.ui \
    aboutDialog.ui
TRANSLATIONS += translations/znotes_ru.ts
RESOURCES += znotes.qrc

# This makes qmake generate translations
isEmpty(QMAKE_LRELEASE):QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
TS_OUT = $$TRANSLATIONS
TS_OUT ~= s/.ts/.qm
TSQM.name = lrelease \
    ${QMAKE_FILE_IN}
TSQM.input = TRANSLATIONS
TSQM.output = $$TS_OUT
TSQM.commands = $$QMAKE_LRELEASE \
    ${QMAKE_FILE_IN}
TSQM.CONFIG = no_link
QMAKE_EXTRA_COMPILERS += TSQM
PRE_TARGETDEPS += $$TS_OUT
!os2:DEFINES += VERSION=\\\"$$VERSION\\\"
unix { 
    PREFIX = $$(PREFIX)
    isEmpty( PREFIX ):PREFIX = /usr
    DEFINES += PROGRAM_DATA_DIR=\\\"$$PREFIX/share/znotes/\\\"
    target.path = $$PREFIX/bin/
    locale.path = $$PREFIX/share/znotes/translations/
    locale.files = $$TS_OUT
    pixmap.path = /usr/share/pixmaps
    pixmap.files = *.png
    desktop.path = /usr/share/applications
    desktop.files = *.desktop
    INSTALLS += target \
        locale \
        pixmap \
        desktop
}
os2 { 
    DEFINES += VERSION=\"$$VERSION\"
    RC_FILE = znotes_os2.rc
}
win32:RC_FILE = znotes.rc
