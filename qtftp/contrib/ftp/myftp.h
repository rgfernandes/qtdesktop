#ifndef MYFTP_H
#define MYFTP_H

#include <QFile>
#include <QFtp>


class MyFtp : public QObject
{
    Q_OBJECT

public:
    MyFtp(QObject *parent = 0);
    bool getFile();

signals:
    void canExit();

private slots:
    void ftpDone(bool error);

private:
    QFtp ftp;
    QFile file;
};

#endif
