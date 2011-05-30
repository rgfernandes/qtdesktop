TEMPLATE = app
QT = gui core
CONFIG += qt warn_on console debug_and_release
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui
HEADERS = src/mainwindowimpl.h \
 src/architemmodel.h \
 src/architem.h \
 src/architempack.h \
 src/archive.h
SOURCES = src/mainwindowimpl.cpp \
 src/main.cpp \
 src/architemmodel.cpp \
 src/architem.cpp \
 src/architempack.cpp \
 src/archive.cpp
RESOURCES += qtarc.qrc
TRANSLATIONS += l10n/qtarc_ru.ts
