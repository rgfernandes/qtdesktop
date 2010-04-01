# -------------------------------------------------
# Project created by QtCreator 2009-02-21T22:08:45
# -------------------------------------------------
TARGET = floaties
SOURCES += floaties.cpp \
    contactwidget.cpp
HEADERS += floaties.h \
    contactwidget.h
INCLUDEPATH += ../../include
unix:INCLUDEPATH += /usr/include
macx:INCLUDEPATH += /opt/local/include
CONFIG += qt plugin
QT += core gui
TEMPLATE = lib

