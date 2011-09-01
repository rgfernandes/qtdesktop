#include "ftpmodel.h"
#include <QtDebug>

FtpHash ftpHash;	// cache of connected ftps

// ----
FtpEngineIterator::FtpEngineIterator(const QString & path, QDir::Filters filters, const QStringList &nameFilters)
	: QAbstractFileEngineIterator(filters, nameFilters),
	index(0) {
	qDebug() << "FTI: " << path;
	// In a real iterator, these entries are fetched from the file system based on the value of path().
	entries << "entry1" << "entry2" << "entry3";
}

bool FtpEngineIterator::hasNext() const {
	return index < entries.size() - 1;
}

QString FtpEngineIterator::next() {
	if (!hasNext())
		return QString();
	++index;
	return currentFilePath();
}

QString FtpEngineIterator::currentFileName() const {
	return entries.at(index);
}

// ----
FtpEngine::FtpEngine(const QString &url) {
	// creating on each path
	// FIXME: port, login, password
	qDebug() << Q_FUNC_INFO;
	QUrl u = QUrl(url);
	QString host(u.host());
	if (!ftpHash.contains(host)) {
		ftpHash[host] = new Ftp();
		if (ftpHash[host]->Connect(host)) {
			qDebug() << "FE: Connected OK";
			/*if (ftp.List()) {
				UrlInfoHash data = ftp.getData();
				QHashIterator<QString, QUrlInfo> i(data);
				while (i.hasNext()) {
					i.next();
					qDebug() << i.key();
				}
			}*/
		} else {
			qDebug() << "FtpEH: Connection failed";
			ftpHash[host]->Disconnect();
		}
	}
	ftp = ftpHash[host];
	qDebug() << "FE: FE()" << url << ", " << u.path();	// path can be empty 
	data = QByteArray();
	path = url;
}

bool FtpEngine::open ( QIODevice::OpenMode mode ) {
	qDebug() << "FE: open()";
	if(mode & QIODevice::Truncate || mode & QIODevice::WriteOnly) return false;
	if(mode & QIODevice::Append) return false;
	// read only so far
	m_pos = 0;
	return true;
}

qint64 FtpEngine::pos () const {
	qDebug() << Q_FUNC_INFO;
	return m_pos;
}

qint64 FtpEngine::read ( char * ddata, qint64 maxlen ) {
	qDebug() << Q_FUNC_INFO;
	maxlen = qBound(qint64(0), data.size()-m_pos, maxlen);
	memcpy(ddata, data.constData()+m_pos, maxlen);
	//qDebug() << "READ:" << maxlen;
	m_pos+=maxlen;
	return maxlen;
}

bool FtpEngine::seek ( qint64 offset ) {
	qDebug() << Q_FUNC_INFO;
	if(offset<0 || offset>=data.size()) return false;
	m_pos = offset;
	return true;
}

qint64 FtpEngine::size () const {
	qDebug() << Q_FUNC_INFO;
	return data.size();
}

bool FtpEngine::close () {
	qDebug() << Q_FUNC_INFO;
	return true;
}
bool FtpEngine::isSequential () const {
	qDebug() << Q_FUNC_INFO;
	return false;
}

bool FtpEngine::supportsExtension ( Extension extension ) const {
	qDebug() << Q_FUNC_INFO << extension;
	return QAbstractFileEngine::supportsExtension(extension);
}

QStringList FtpEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const {
	qDebug() << Q_FUNC_INFO;
	return QStringList();
}

QAbstractFileEngineIterator * FtpEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames) {
	qDebug() << Q_FUNC_INFO;
	return new FtpEngineIterator(path, filters, filterNames);
}

// ----
QAbstractFileEngine * FtpEngineHandler::create ( const QString & url ) const {
	//qDebug() << "FEH create";
	return url.startsWith("ftp://") ? new FtpEngine(url) : NULL;
}
