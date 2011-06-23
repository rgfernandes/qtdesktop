#ifndef __FTP_H__
#define __FTP_H__

#include <QtCore>
#include <QtNetwork>

class Ftp : public QObject
{
	Q_OBJECT
public:
	Ftp(void);
	void cdToParent();
private slots:
	void connectOrDisconnect();
	void downloadFile();
	void ftpCommandFinished(int commandId, bool error);
	void addToList(const QUrlInfo &urlInfo);
	void processItem(QTreeWidgetItem *item, int column);
	void enableDownload();
	void enableConnect();
private:
	QTreeWidget *fileList;
	bool cdToParentEnabled;
	bool connectEnabled;
	bool downloadEnabled;
	QHash<QString, bool> isDirectory;
	QString currentPath;
	QFtp *ftp;
	QFile *file;
	QNetworkSession *networkSession;
};

#endif // __FTP_H__
