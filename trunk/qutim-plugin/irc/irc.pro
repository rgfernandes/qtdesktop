TEMPLATE = lib
TARGET = irc
OBJECTS_DIR = build/obj
UI_DIR = build/uic
MOC_DIR = build/moc
RCC_DIR = build/rcc
INCLUDEPATH += ../../include \
    /usr/include
CONFIG += qt \
    plugin
QT += core \
    gui \
    network \
    xml
RESOURCES += irc.qrc
HEADERS += irclayer.h \
    settings/ircsettings.h \
    addaccountform.h \
    ircaccount.h \
    accountbutton.h \
    ircpluginsystem.h \
    ircconsole.h \
    ircprotocol.h \
    joinchannel.h \
    listchannel.h \
    settings/accountsettings.h \
    textdialog.h \
    ircavatar.h
SOURCES += irclayer.cpp \
    settings/ircsettings.cpp \
    addaccountform.cpp \
    ircaccount.cpp \
    accountbutton.cpp \
    ircpluginsystem.cpp \
    ircconsole.cpp \
    ircprotocol.cpp \
    joinchannel.cpp \
    listchannel.cpp \
    settings/accountsettings.cpp \
    textdialog.cpp \
    ircavatar.cpp
FORMS += settings/ircsettings.ui \
    addaccountform.ui \
    ircconsole.ui \
    joinchannel.ui \
    listchannel.ui \
    settings/accountsettings.ui \
    textdialog.ui
