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

class Ftp : public QObject
{
	Q_OBJECT
public:
	Ftp(void);
	void Connect(const QString &);
	void Disconnect();
	void DirCd(QString &);
	void FileGet(QString &);
	bool isConnected(void) { return connected; }
	int getState(void) { return ftp->state(); }
private slots:
	void ftpCommandFinished(int commandId, bool error);
	//void ftpStateChanged(int);
	//void ftpCommandStarted(int);
	//void ftpDone(int);
	void addToList(const QUrlInfo &urlInfo);
	void enableConnect();
private:
	QHash<QString, QUrlInfo> fileList;
	bool connected;
	QString currentPath, currentHost;
	QFtp *ftp;
	QFile *file;
	QNetworkSession *networkSession;
};

#endif // __FTP_H__
