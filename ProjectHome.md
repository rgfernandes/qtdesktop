The main goal of [QtDesktop](http://www.qtdesktop.org) project is to get all usual applications, X-platform (Linuxnix/Windows) and Qt4-based.

# Tasks #
  * make packages for all mainstream distros (now - Fedora, OpenSUSE, Mandriva, Debian) and Windows;
  * testing of packages in target distros;
  * work out unified "standards":
    * what to use - qmake or cmake?
    * unified %{name}.pro and/or CmakeList.txt;
    * freedesktop.org-compatibility (/usr/share/applications/%{name}.desktop and /usr/share/pixmaps/%{name}.png);
    * unified and freedesktop.org-compatible %{name}.desktop;
    * where put translations to?
  * and so on

# Table of content #
  * [QtDesktop Packaging Style](PackagingStyle.md)
  * [freedesktop.org categories](Categories.md)
  * [OpenSUSE Categories](ObsCats.md)
  * [QtDesktop filemanager/openfile dialog concept](FmConcept.md)

# Applications #
## Trunk ##
Live/developing projects:
  * [QMailClient](http://qtdesktop.googlecode.com/svn/trunk/qmailclient/) - Qt Mail client.
  * [QRDC](http://qtdesktop.googlecode.com/svn/trunk/qrdc/) - Qt Remote Desktop Connection.
  * [QtRun](http://qtdesktop.googlecode.com/svn/trunk/qtarc/) - Qt Archiver (powered by libarchive and QFileEngineHandler).
  * [QtFTP](http://qtdesktop.googlecode.com/svn/trunk/qtftp/) - Qt FTP client (powered by QFtp and QFileEngineHandler).
  * [QtPDFView](http://qtdesktop.googlecode.com/svn/trunk/qtrun/) - Qt PDF viewer (powered by poppler).
  * [QtRun](http://qtdesktop.googlecode.com/svn/trunk/qtrun/) - Qt Running application.
  * [zNotes](http://qtdesktop.googlecode.com/svn/trunk/znotes/)

## Branches ##
Paused/testing projects:
  * [QGooglePIM](http://qtdesktop.googlecode.com/svn/branches/qgooglepimc/) - Personal Information Manager based on Google's services.
  * [Google Code's Issues client](http://qtdesktop.googlecode.com/svn/branches/qissues/).
  * [Qt FUSE frontend](http://qtdesktop.googlecode.com/svn/branches/qtfuse/).
  * [Qt Personal Information Manager](http://qtdesktop.googlecode.com/svn/branches/qtpim/).
  * [Qt Selta](http://qtdesktop.googlecode.com/svn/branches/qtselta/) - frontend to Etersoft's Selta.
  * [Qt Xattr manager](http://qtdesktop.googlecode.com/svn/branches/qxattr/) - manage ext3 xattr.
  * [Research Assistant](http://qtdesktop.googlecode.com/svn/branches/rassistant/).

## Tags ##
RIP/unusual projects: