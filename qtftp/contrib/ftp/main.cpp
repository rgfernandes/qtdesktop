#include <QtCore>

#include "myftp.h"
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    MyFtp ftpclient;
    if(!ftpclient.getFile())
    return 1;
    QObject::connect(&ftpclient, SIGNAL(canExit()), &app, SLOT(quit()));
    return app.exec();
}

