/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPointer>
#include <QMap>
#include <QTreeWidget>

#include "ui_mainwindow.h"
#include "projectproperty.h"
#include "settings.h"
#include "config.h"
#include "raedit.h"
#include "projectsources.h"
#include "fontsettingsdialog.h"
#include "statistics.h"
#include "autotext.h"

class TabbedBrowser;
class HelpDialog;
class HelpWindow;
class QMenu;
class QDockWidget;


//====================== class MainWindow ============================

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
	Ui::MainWindow ui;

    TabbedBrowser *browsers() const;
	HelpDialog *helpDialog() const;

    void setupPopupMenu(QMenu *menu);
    void removePendingBrowser(HelpWindow *win);
    void msgReloadRequest();
    void OpenInExternalApplication(QString app, QString FileName);
    void addSignature(QString str);

signals:
	void saveOpenedLink(); //used to save opened file before open a new one
	
public slots:
    MainWindow *newWindow();

    void setup();
    void showLink(const QString &link);
    void showLinks(const QStringList &links);
    void saveSettings();
    void updateBookmarkMenu();
    void ProjectOpen(QString fileName);
    void setMenuSign();
    void setMenuSign(int defaultSignIndex);
    void insertSignature(QAction *a);
 	void insertDefaultSignature();
    void updateAppFont(FontSettings settings);
    void projectModified(bool modified);
 	
private slots:
    void ProjectNew();
    void ProjectNewDiary();
    void ProjectNewNotebook();
    void createProject(QString prjTitle, QString prjFN, QString prjStartPage);
    void updateProjectProperties(QString prjTitle, QString prjFN, QString prjStartPage);
    void ProjectOpen();
    void ProjectSaveAs();
    void ProjectBackup();
    void ProjectProps();
	void showStatistics();
    void ProjectSrc();
    void setLangEn();
    void setLangRu();
    void showAppSettings();
    void showAutoText();
    void globalShortcut_CtrlShiftInsert();
    
	void on_actionNewWindow_triggered();
//    void on_actionGoHome_triggered();
    void on_actionFilePrint_triggered();
//    void on_actionClose_triggered();
    void on_actionHelpWhatsThis_triggered();
//    void on_actionHelpAssistant_triggered();
    void on_actionAboutAssistant_triggered();
    void on_actionSaveFileAs_triggered();
    void on_actionSyncToc_triggered();

    void about();
    void setupBookmarkMenu();
    void showBookmark(QAction *action);
    void showLinkFromClient(const QString &link);
//    void showQtHelp();
    void showSearchLink(const QString &link, const QStringList &terms);
//    void showGoActionLink();
    void updateProfileSettings();
//    void backwardAvailable(bool);	
//    void forwardAvailable(bool);

    void browserTabChanged();
//    void copyAvailable(bool yes);
    void updateTabActions(int index);
    //void showFontSettingsDialog();
    void exitApp();

protected:
    void closeEvent(QCloseEvent *);
    void timerEvent(QTimerEvent *);

private:
//    bool insertActionSeparator();
    QList<QAction*> goActions;
    uint setupCompleted:1;
    QMap<QAction*, QString> bookmarks;
    TabbedBrowser *tabs;
	ProjectProperties *prjprop;
	ProjectSources *prjsrc;
	ProjectStatistics *prjstat;
    AutoText *autotxt;
	AppSettings *appsets;
    HelpDialog *helpDock;
    QDockWidget *dw;
    static QList<MainWindow*> windows;
//    QMap<QAction*,QString> *goActionDocFiles;
    QList<QPair<HelpWindow*,QString> > pendingBrowsers;
    QMenu *menuSign;
};

#endif // MAINWINDOW_H
