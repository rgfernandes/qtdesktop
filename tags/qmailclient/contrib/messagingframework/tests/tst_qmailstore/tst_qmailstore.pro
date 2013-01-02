CONFIG += qtestlib unittest

include(../../common.pri)

QT += sql
TEMPLATE = app
TARGET = tst_qmailstore
target.path += $$QMF_INSTALL_ROOT/tests
INSTALLS += target

QTOPIAMAIL=../../src/libraries/qtopiamail

DEPENDPATH += .
INCLUDEPATH += . $$QTOPIAMAIL $$QTOPIAMAIL/support
LIBS += -L$$QTOPIAMAIL -lqtopiamail
QMAKE_LFLAGS += -Wl,-rpath,$$QTOPIAMAIL

SOURCES += tst_qmailstore.cpp

