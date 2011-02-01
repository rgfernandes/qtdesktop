isEmpty( PREFIX ) {
    PREFIX=/usr
}
QT       += core gui network dbus
CONFIG += qt warn_on link_pkgconfig release debug
PKGCONFIG += liblightdm-qt-0
TARGET = qtgreeter
TEMPLATE = app
SOURCES += qtgreater.cpp
HEADERS  += qtgreater.h
#HEADERPATH += `pkg-config --cflags liblightdm-qt-0`
FORMS    += qtgreater.ui
