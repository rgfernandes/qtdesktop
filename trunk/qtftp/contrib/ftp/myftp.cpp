#include <QtCore>
#include <QFtp>
#include <iostream>

#include "myftp.h"
using namespace std;

MyFtp::MyFtp(QObject *parent)
    : QObject(parent)
{
    connect(&ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
}

bool MyFtp::getFile()
{
file.setFileName("ftpget.txt");
if (!file.open(QIODevice::WriteOnly))
    {
        cerr << "Error: Cannot open local file" << endl;
        return false;
    }
ftp.connectToHost("ftp.trolltech.com");
ftp.login();
ftp.cd("qt");
ftp.get("INSTALL", &file);
ftp.close();
return true;
}

void MyFtp::ftpDone(bool error)
{
    if (error) 
    {
        cerr << "Error: " << qPrintable(ftp.errorString()) << endl;
    } 
    else 
    {
        cerr << "File downloaded" <<  endl;
    }
    file.close();
    emit canExit();
}