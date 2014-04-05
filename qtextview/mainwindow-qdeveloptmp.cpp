#include <QtGui>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSlots(void)
{
    connect(ui->action_FileOpen, SIGNAL(triggered()), this, SLOT(onActionFileOpen()));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(onActionAbout()));
    connect(ui->action_AboutQt, SIGNAL(triggered()), this, SLOT(onActionAboutQt()));
}

void MainWindow::fileOpen(const QString &path)
{
	
    //qDebug() << fileName;
    //ui->textBrowser->setSource(QUrl::fromLocalFile(fileName));
    QFile file(path);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString src = QString::fromUtf8(file.readAll());
        ui->textBrowser->setPlainText(src);
    }
}

void MainWindow::onActionFileOpen(void)
{
    QString fileName = QFileDialog::getOpenFileName(0, tr("Open file"), QString(), tr("Text file") + " (*.txt *.html)");
        if (!fileName.isEmpty()) {
            fileOpen(fileName);
        }
}

void MainWindow::onActionAbout(void)
{
    QMessageBox::about(this, tr("About Simple Text Viewer"),
                            tr("Simple text viewer."));
}

void MainWindow::onActionAboutQt(void)
{
    QMessageBox::aboutQt(this);
}
