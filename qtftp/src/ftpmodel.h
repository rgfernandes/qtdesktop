#ifndef __FTPMODEL_H__
#define __FTPMODEL_H__

#include <QtCore>
#include <QtNetwork>

#include "ftp.h"

class FtpEngineHandler : public QObject, QAbstractFileEngineHandler
{
	Q_OBJECT
public:
	QAbstractFileEngine * create ( const QString & ) const;
	bool setHost(const QString &);
private:
	Ftp ftp;
};

class FtpEngine : public QAbstractFileEngine
{
public:
	FtpEngine(const QString &);
	bool open ( QIODevice::OpenMode mode );
	qint64 pos () const;
	qint64 read ( char * data, qint64 maxlen );
	bool seek ( qint64 offset );
	qint64 size () const;
	bool close ();
	bool isSequential () const;
	bool supportsExtension ( Extension extension ) const;
private:
	QByteArray data;
	qint64 m_pos;
};

#endif // __FTPMODEL_H__
