#include <QtGui/QApplication>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    if (a.arguments().count() > 1) {
        w.fileOpen(a.arguments().at(1));
    }
    return a.exec();
}
