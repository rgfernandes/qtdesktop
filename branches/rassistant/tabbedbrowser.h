/****************************************************************************
**
** Copyright (C) 2007-2008 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on Trolltech Qt Assistant 4.3, http://trolltech.com
****************************************************************************/
#ifndef TABBEDBROWSER_H
#define TABBEDBROWSER_H

#include "ui_tabbedbrowser.h"

class MainWindow;
class HelpWindow;
class QStyleSheet;
class QMimeSourceFactory;
class QTimer;

class TabbedBrowser : public QWidget
{
    Q_OBJECT
public:
    TabbedBrowser(MainWindow *parent);
    virtual ~TabbedBrowser();

    MainWindow *mainWindow() const;
    HelpWindow *currentBrowser() const;
    QStringList sources() const;
    QList<HelpWindow*> browsers() const;

    HelpWindow* newBackgroundTab();
    HelpWindow* createHelpWindow();

    void setAppTitle(HelpWindow*, const QString &);

signals:
    void tabCountChanged(int count);
    void browserUrlChanged(const QString &link);
    void showDocProperties();

protected:
        void keyPressEvent(QKeyEvent *);
        bool eventFilter(QObject *o, QEvent *e);

public slots:
    void init();
    void forward();
    void backward();
    void setSource(const QString &ref);
    void reload();
    void home();
    void nextTab();
    void previousTab();
    void newTab(const QString &lnk);
    void zoomIn();
    void zoomOut();
    void updateTitle(const QString &title);
    void newTab();
    void transferFocus();
    void initHelpWindow(HelpWindow *win);
    void setup();
    void copy();
    void closeTab();
    void closeAllTabs();
    void sourceChanged();

    void find();
    void findNext();
    void findPrevious();

private slots:
    void find(QString, bool forward = false, bool backward = false);
    void openTabMenu(const QPoint& pos);

private:
    Ui::TabbedBrowser ui;
    QWidget *lastCurrentTab;
    QFont tabFont;

    QString fixedFontFam;
    QColor lnkColor;
    bool underlineLnk;
        QTimer *autoHideTimer;
};

#endif // TABBEDBROWSER_H
