TEMPLATE = lib 
CONFIG += warn_on

include(../../../common.pri)

TARGET = qmfutil 
target.path += $$QMF_INSTALL_ROOT/lib
INSTALLS += target

DEFINES += QMFUTIL_INTERNAL
symbian: {
	MMP_RULES += EXPORTUNFROZEN
}

DEPENDPATH += .

INCLUDEPATH += . ../qtopiamail ../qtopiamail/support

LIBS += -L../qtopiamail -lqtopiamail

HEADERS += emailfoldermodel.h \
           emailfolderview.h \
           folderdelegate.h \
           foldermodel.h \
           folderview.h \
           qmailcomposer.h \
           qmailviewer.h \
           selectfolder.h

SOURCES += emailfoldermodel.cpp \
           emailfolderview.cpp \
           folderdelegate.cpp \
           foldermodel.cpp \
           folderview.cpp \
           qmailcomposer.cpp \
           qmailviewer.cpp \
           selectfolder.cpp

TRANSLATIONS += libqmfutil-ar.ts \
                libqmfutil-de.ts \
                libqmfutil-en_GB.ts \
                libqmfutil-en_SU.ts \
                libqmfutil-en_US.ts \
                libqmfutil-es.ts \
                libqmfutil-fr.ts \
                libqmfutil-it.ts \
                libqmfutil-ja.ts \
                libqmfutil-ko.ts \
                libqmfutil-pt_BR.ts \
                libqmfutil-zh_CN.ts \
                libqmfutil-zh_TW.ts

RESOURCES += qmfutil.qrc

include(../../common.pri)

