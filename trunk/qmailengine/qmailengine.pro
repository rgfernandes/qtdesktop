QT += core gui network
TEMPLATE = lib
VERSION = 0.1.0
TARGET = qmailengine
CONFIG += dll thread release debug
SOURCES += src/mail.cpp src/pop3.cpp src/smtp.cpp src/mime.cpp
HEADERS += src/mail.h src/pop3.h src/smtp.h src/mime.h
MOC_DIR = build
OBJECTS_DIR = build

isEmpty( PREFIX ):INSTALL_PREFIX = /usr
else:INSTALL_PREFIX = $${PREFIX}

target.path = $${INSTALL_PREFIX}/lib

headers.files = $${HEADERS} include/QMailEngine
headers.path = $${INSTALL_PREFIX}/include/QMailEngine

features.files += qmailengine.prf
features.path = $$[QMAKE_MKSPECS]/features

INSTALLS += target headers features
