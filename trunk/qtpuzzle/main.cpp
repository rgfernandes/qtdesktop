#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //The following line is used to initialize the background of the window and the color of the buttons

    a.setStyleSheet("QMainWindow {  background-image: url(blur.jpg);}  QPushButton { color:  rgb(25, 25, 112);}");

    return a.exec();
}
