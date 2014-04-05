#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include <QMimeDatabase>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void fileOpen(const QString &);

private slots:
    void    onActionFileOpen(void);
    void    onActionFileOpenNext(void);
    void    onActionFileOpenPrev(void);
    void    onActionAbout(void);
    void    onActionAboutQt(void);

private:
    QFileInfo   *fileInfo;  // current opened file
    QStringList *fileList;  // siblings
    QStringList dirfilter;
    QMimeDatabase mimedb;

    Ui::MainWindow *ui;
    void setSlots(void);
    void loadFile(void);
};

#endif // MAINWINDOW_H
