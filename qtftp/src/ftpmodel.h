#ifndef __FTPMODEL_H__
#define __FTPMODEL_H__

#include <QtCore>
#include <QtNetwork>

#include "ftp.h"

typedef QHash<QString, Ftp *> FtpHash;

class FtpEngineIterator : public QAbstractFileEngineIterator {
public:
	FtpEngineIterator(const QString &, QDir::Filters, const QStringList &);
	QString next();
	bool hasNext() const;
	QString currentFileName() const;
private:
	QStringList entries;
	int index;
};

class FtpEngine : public QAbstractFileEngine {
public:
	FtpEngine(const QString &);
	~FtpEngine() { }
	bool open ( QIODevice::OpenMode mode );
	qint64 pos () const;
	qint64 read ( char * data, qint64 maxlen );
	bool seek ( qint64 offset );
	qint64 size () const;
	bool close ();
	bool isSequential () const;
	bool supportsExtension ( Extension extension ) const;
	Iterator * beginEntryList(QDir::Filters, const QStringList &);
private:
	Ftp *ftp;
	QString path;
	QByteArray data;
	qint64 m_pos;
};

class FtpEngineHandler : public QObject, QAbstractFileEngineHandler {
	Q_OBJECT
public:
	QAbstractFileEngine * create ( const QString & ) const;
};

#endif // __FTPMODEL_H__
