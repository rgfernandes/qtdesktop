#ifndef __FTP_H__
#define __FTP_H__
/* TODO:
 * add signals:
	- connected/disconnected
	- ready/notready
 * commands:
	- session: open, close
	- dir: cd, pwd, ls, mkdir, rmdir
	- file: get, put, del
*/

#include <QtCore>
#include <QtNetwork>

typedef QHash<QString, QUrlInfo> UrlInfoHash;

class Ftp : public QObject
{
	Q_OBJECT
public:
	Ftp(void);
	// host
	bool Connect(const QString &, int timeout = 10);
	void Disconnect();
	// dir
	bool Cd(const QString & dir = QString());
	bool List(const QString & dir = QString());
	bool MkDir(const QString &);
	bool RmDir(const QString &);
	// file
	bool Get(const QString &);
	//bool Put(QIODevice *, const QString &);
	bool Rename(const QString &, const QString &);
	bool Remove(const QString &);
	// misc
	bool isConnected(void) { return connected; }
	int getState(void) { return ftp->state(); }
	UrlInfoHash &getData(void) { return fileList; }
private slots:
	void ftpCommandFinished(int commandId, bool error);
	void ftpStateChanged(int);
	void addToList(const QUrlInfo &urlInfo);
	void enableConnect();
private:
	UrlInfoHash fileList;
	bool connected, ready;
	QString currentPath, currentHost;
	QFtp *ftp;
	QFile *file;
	QNetworkSession *networkSession;
	bool wait4ready(const int timeout = 5);
};

#endif // __FTP_H__
