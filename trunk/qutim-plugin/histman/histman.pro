TEMPLATE = lib
CONFIG += qt \
    plugin
INCLUDEPATH += ../../include /usr/include
TARGET = histman
QT += core \
    gui \
	xml \
	sql
HEADERS += clients/miranda.h \
    clients/licq.h \
    clients/andrq.h \
    clients/qipinfium.h \
    clients/qippda.h \
    clients/sim.h \
    clients/pidgin.h \
    clients/kopete.h \
    clients/qip.h \
    clients/qutim.h \
    src/historymanagerplugin.h \
    src/k8json.h \
    include/qutim/historymanager.h \
    src/historymanagerwindow.h \
    src/chooseclientpage.h \
    src/clientconfigpage.h \
    src/importhistorypage.h \
    src/dumphistorypage.h \
    src/chooseordumppage.h \
    clients/psi.h \
    clients/gajim.h
SOURCES += clients/miranda.cpp \
    clients/licq.cpp \
    clients/andrq.cpp \
    clients/qipinfium.cpp \
    clients/qippda.cpp \
    clients/sim.cpp \
    clients/pidgin.cpp \
    clients/kopete.cpp \
    clients/qutim.cpp \
    clients/qip.cpp \
    src/historymanagerplugin.cpp \
    src/k8json.cpp \
    src/historymanagerwindow.cpp \
    src/chooseclientpage.cpp \
    src/clientconfigpage.cpp \
    src/importhistorypage.cpp \
    src/dumphistorypage.cpp \
    src/chooseordumppage.cpp \
    clients/psi.cpp \
    clients/gajim.cpp
FORMS += src/chooseclientpage.ui \
    src/clientconfigpage.ui \
    src/importhistorypage.ui \
    src/dumphistorypage.ui \
    src/chooseordumppage.ui
