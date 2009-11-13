CONFIG += dll thread release
HEADERS += src/mail.h src/pop3.h src/smtp.h src/mime.h
QT += core gui network
SOURCES += src/mail.cpp src/pop3.cpp src/smtp.cpp src/mime.cpp
TEMPLATE = lib
MOC_DIR = build
OBJECTS_DIR = build
