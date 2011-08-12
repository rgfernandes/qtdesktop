TEMPLATE = app
LANGUAGE = C++
TARGET = bsatest
CONFIG += qt \
 thread \
 warn_on \
 console \
 debug_and_release \
 build_all
HEADERS += bsa.h fsengine.h fsmanager.h
SOURCES += bsa.cpp fsengine.cpp fsmanager.cpp
QT += core gui
