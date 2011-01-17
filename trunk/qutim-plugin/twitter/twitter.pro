QT += network \
    script
TARGET = twitter
TEMPLATE = lib
CONFIG += qt \
    plugin
INCLUDEPATH += ../../include
unix:INCLUDEPATH += /usr/include
macx:INCLUDEPATH += /opt/local/include

# Input
HEADERS += src/twlayer.h \
    src/twpluginsystem.h \
    src/twaccount.h \
    src/loginform.h \
    src/twstatusobject.h \
    src/twapiwrap.h \
    src/twcontactlist.h \
    src/twavatarmanagement.h

# FORMS += core/edditaccount.ui \
SOURCES += src/twlayer.cpp \
    src/twpluginsystem.cpp \
    src/twaccount.cpp \
    src/loginform.cpp \
    src/twstatusobject.cpp \
    src/twapiwrap.cpp \
    src/twcontactlist.cpp \
    src/twavatarmanagement.cpp
RESOURCES += src/twitter.qrc
FORMS += src/loginform.ui
