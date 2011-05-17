include(fontpanel/fontpanel.pri)
TEMPLATE = app
LANGUAGE = C++
TARGET = ra
CONFIG += qt \
    warn_on \
    release
win32 { 
    LIBS += -lshell32
    RC_FILE = ra.rc
}
QT += xml \
    core \
    gui \
    sql
PROJECTNAME = Research \
    Assistant
DESTDIR = build/bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build
FORMS += ui/autoitems.ui \
    ui/docproperty.ui \
    ui/helpdialog.ui \
    ui/itemproperty.ui \
    ui/linkproperty.ui \
    ui/mainwindow.ui \
    ui/projectproperty.ui \
    ui/projectsources.ui \
    ui/settings.ui \
    ui/tabbedbrowser.ui \
    ui/tableproperty.ui \
    ui/topicchooser.ui \
    ui/cellsplit.ui \
    ui/imageproperty.ui \
    ui/statistics.ui \
    ui/autotext.ui
SOURCES += autoitems.cpp \
    config.cpp \
    docproperty.cpp \
    docuparser.cpp \
    fontsettingsdialog.cpp \
    helpdialog.cpp \
    helpwindow.cpp \
    index.cpp \
    itemproperty.cpp \
    linkproperty.cpp \
    main.cpp \
    mainwindow.cpp \
    pcommon.cpp \
    profile.cpp \
    projectproperty.cpp \
    projectsources.cpp \
    raedit.cpp \
    settings.cpp \
    srclistparser.cpp \
    tabbedbrowser.cpp \
    tableproperty.cpp \
    topicchooser.cpp \
    cellsplit.cpp \
    imageproperty.cpp \
    statistics.cpp \
    autotext.cpp
HEADERS += autoitems.h \
    config.h \
    docproperty.h \
    docuparser.h \
    fontsettingsdialog.h \
    helpdialog.h \
    helpwindow.h \
    index.h \
    itemproperty.h \
    linkproperty.h \
    mainwindow.h \
    pcommon.h \
    profile.h \
    projectproperty.h \
    projectsources.h \
    raedit.h \
    settings.h \
    srclistparser.h \
    tabbedbrowser.h \
    tableproperty.h \
    topicchooser.h \
    cellsplit.h \
    imageproperty.h \
    statistics.h \
    autotext.h
RESOURCES += ra.qrc
TRANSLATIONS += lang/ra_rus.ts
OTHER_FILES += to-do.txt
