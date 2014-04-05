// text/plain: ÑÐµÐºÑÑÐ¾Ð²ÑÐ¹ Ð´Ð¾ÐºÑÐ¼ÐµÐ½Ñ (*.txt *.asc *,v *.doc *.ini *.inf)
// text/html: Ð´Ð¾ÐºÑÐ¼ÐµÐ½Ñ HTML (*.html *.htm)
// TODO:
// - create filelist of text/plain inherited files in folder (in thread)
// - encoding

#ifdef HAVE_QT5
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QtCore>
#include <QMimeType>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exts.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setSlots();
    fileInfo = new QFileInfo();
    fileList = new QStringList();
    for (unsigned int i=0; exts[i]; i++)
        dirfilter << exts[i];
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSlots(void)
{
    connect(ui->action_FileOpen,            SIGNAL(triggered()), this, SLOT(onActionFileOpen()));
    connect(ui->action_FileOpenNext,        SIGNAL(triggered()), this, SLOT(onActionFileOpenNext()));
    connect(ui->action_FileOpenPrevious,    SIGNAL(triggered()), this, SLOT(onActionFileOpenPrev()));
    connect(ui->action_About,               SIGNAL(triggered()), this, SLOT(onActionAbout()));
    connect(ui->action_AboutQt,             SIGNAL(triggered()), this, SLOT(onActionAboutQt()));
}

void MainWindow::loadFile(void)
{
    QFile file(fileInfo->absoluteFilePath());
    if (file.open(QFile::ReadOnly | QFile::Text)) {
	// 1. check mimetype
	QMimeType mime = mimedb.mimeTypeForFile(*fileInfo);
	qDebug(mime.name().toLocal8Bit());
	qDebug(mime.filterString().toUtf8());
	// 2. check encoding
	// 3. process
        QString src = QString::fromUtf8(file.readAll());
	if (mime.inherits("text/html")) {
		ui->textBrowser->setHtml(src);
		//qDebug("Das ist html");
	} else {
		ui->textBrowser->setPlainText(src);
		//qDebug("Das ist text");
	};
        setWindowTitle(QString("QTextView: " + fileInfo->fileName()));
    }
}

void MainWindow::fileOpen(const QString &path)
{
    /*
      * get absolute file path
      * get it's folder
      * get all files in folder
      * get current file's position
      * lets go
    */
    fileInfo->setFile(path);
    QDir d = fileInfo->dir();      // parent folder
    d.setNameFilters(dirfilter);
    fileList->clear();
    *fileList << d.entryList(dirfilter, QDir::Files, QDir::Name);
    int i = fileList->indexOf(fileInfo->fileName());
    ui->action_FileOpenNext->setEnabled((i + 1) < fileList->count());
    ui->action_FileOpenPrevious->setEnabled(i > 0);
    loadFile();
}

void MainWindow::onActionFileOpen(void)
{
    QString fileName = QFileDialog::getOpenFileName(0, tr("Open file"), QString(), tr("Text file") + " (*.txt *.html)");
        if (!fileName.isEmpty()) {
            fileOpen(fileName);
        }
}

void MainWindow::onActionFileOpenNext(void)
{
    int i = fileList->indexOf(fileInfo->fileName());
    if ((i + 1) < fileList->count()) {
        i++;
        ui->action_FileOpenPrevious->setEnabled(i > 0);
        ui->action_FileOpenNext->setEnabled((i + 1) < fileList->count());
        fileInfo->setFile(fileInfo->dir().canonicalPath() + "/" + fileList->at(i));
        loadFile();
    }
}

void MainWindow::onActionFileOpenPrev(void)
{
    int i = fileList->indexOf(fileInfo->fileName());
    if (i > 0) {
        i--;
        ui->action_FileOpenPrevious->setEnabled(i > 0);
        ui->action_FileOpenNext->setEnabled((i + 1) < fileList->count());
        fileInfo->setFile(fileInfo->dir().canonicalPath() + "/" + fileList->at(i));
        loadFile();
    }
}

void MainWindow::onActionAbout(void)
{
    QMessageBox::about(this, tr("About Simple Text Viewer"), tr("Simple text viewer."));
}

void MainWindow::onActionAboutQt(void)
{
    QMessageBox::aboutQt(this);
}
