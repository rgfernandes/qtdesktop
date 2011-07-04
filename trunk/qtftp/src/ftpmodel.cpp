#include "ftpmodel.h"
#include <QtDebug>

QAbstractFileEngine * FtpEngineHandler::create ( const QString & url ) const {
	return (!url.startsWith("ftp:")) ? 0 : new FtpEngine(url);
}

bool FtpEngineHandler::setHost(const QString &hostname) {
	//this->connect(&ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
	qDebug() << "FtpEH: Connecting...";
	if (ftp.Connect(hostname)) {
		qDebug() << "FtpEH: Connected OK";
		if (ftp.List()) {
			UrlInfoHash data = ftp.getData();
			QHashIterator<QString, QUrlInfo> i(data);
			while (i.hasNext()) {
				i.next();
				qDebug() << i.key();
			}
		}
	} else {
		qDebug() << "FtpEH: Connection failed";
		ftp.Disconnect();
	}
	return true;
}

// ----
FtpEngine::FtpEngine(const QString &url) {
	data = QByteArray();
}

bool FtpEngine::open ( QIODevice::OpenMode mode ) {
	if(mode & QIODevice::Truncate || mode & QIODevice::WriteOnly) return false;
	if(mode & QIODevice::Append) return false;
	// read only so far
	m_pos = 0;
	return true;
}
qint64 FtpEngine::pos () const {
	return m_pos;
}
qint64 FtpEngine::read ( char * ddata, qint64 maxlen ) {
	//qDebug() << Q_FUNC_INFO << m_pos << maxlen;
	maxlen = qBound(qint64(0), data.size()-m_pos, maxlen);
	memcpy(ddata, data.constData()+m_pos, maxlen);
	//qDebug() << "READ:" << maxlen;
	m_pos+=maxlen;
	return maxlen;
}

bool FtpEngine::seek ( qint64 offset ) {
	//qDebug() << Q_FUNC_INFO << offset;
	if(offset<0 || offset>=data.size()) return false;
	m_pos = offset;
	return true;
}

qint64 FtpEngine::size () const {
	return data.size();
}

bool FtpEngine::close () {
//    qDebug() << Q_FUNC_INFO;
	return true;
}
bool FtpEngine::isSequential () const {
//    qDebug() << Q_FUNC_INFO;
	return false;
}

bool FtpEngine::supportsExtension ( Extension extension ) const {
//    qDebug() << Q_FUNC_INFO << extension;
	return QAbstractFileEngine::supportsExtension(extension);
}
