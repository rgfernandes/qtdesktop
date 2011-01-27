VERSION = 0.1.0
TEMPLATE = app
QT += gui core network sql
SOURCES += src/main.cpp \
 src/mainwinimpl.cpp \
 src/configdialogimpl.cpp \
 src/maileditordialogimpl.cpp \
 src/dummy.cpp \
 src/mailbox.cpp \
 src/accountsdialogimpl.cpp
HEADERS += src/mainwinimpl.h \
 src/configdialogimpl.h \
 src/maileditordialogimpl.h \
 src/dummy.h \
 src/mailbox.h \
 src/accountsdialogimpl.h
LIBS += `pkg-config --libs vmime`
FORMS += ui/MailEditorDialog.ui \
 ui/MainWin.ui \
 ui/ConfigDialog.ui \
 ui/SettingsDialog.ui \
 ui/AccountsDialog.ui
TARGET = qmailclient
PROJECTNAME = qmailclient
MOC_DIR = build
OBJECTS_DIR = build
RESOURCES += resources.qrc
TRANSLATIONS += translations/qmailclient_ru.ts
unix {
 isEmpty(PREFIX) {
  PREFIX =   /usr
 }
 isEmpty(BINDIR) {
  BINDIR =   $$PREFIX/bin
 }
 isEmpty(DATAROOT) {
  DATAROOT =   $$PREFIX/share
 }
 isEmpty(DESKTOPDIR) {
  DESKTOPDIR =   $$DATAROOT/applications
 }
 isEmpty(PIXMAPSDIR) {
  PIXMAPSDIR =   $$DATAROOT/pixmaps
 }
 target.path =  $$BINDIR
 desktop.path =  $$DESKTOPDIR
 desktop.files =  qmailclient.desktop
 icons.path =  $$PIXMAPSDIR
 icons.files =  qmailclient.png
 translations.path =  $$DATAROOT/qmailclient/translations
 INSTALLS +=  target  desktop  icons  translations
}
CONFIG += debug_and_release
