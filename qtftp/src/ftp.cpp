#include <QtGui>
#include <QtNetwork>
#include <QtTest/QtTest>

#include "ftp.h"

Ftp::Ftp(void) : ftp(0), networkSession(0) {
	QNetworkConfigurationManager manager;
	if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
		// Get saved network configuration
		QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
		settings.beginGroup(QLatin1String("QtNetwork"));
		const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
		settings.endGroup();
		// If the saved network configuration is not currently discovered use the system default
		QNetworkConfiguration config = manager.configurationFromIdentifier(id);
		if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered)
			config = manager.defaultConfiguration();
		networkSession = new QNetworkSession(config, this);
		connect(networkSession, SIGNAL(opened()), this, SLOT(enableConnect()));
		connected = false;
		qDebug() << "FTP: Opening session...";
		networkSession->open();
	}
}

bool	Ftp::Connect(const QString &urlpath, int timeout)
{
	connected = false;
	// if connectEnabled...
	currentHost = urlpath;
	ftp = new QFtp(this);
	connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCommandFinished(int,bool)));
	connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(addToList(QUrlInfo)));
	connect(ftp, SIGNAL(stateChanged(int)), this, SLOT(ftpStateChanged(int)));
	//connect(ftp, SIGNAL(commandStarted(int)), this, SLOT(ftpCommandStarted(int)));
	//connect(ftp, SIGNAL(done(int)), this, SLOT(ftpDone(int)));
	fileList.clear();
	currentPath.clear();
	QUrl url(urlpath);
	if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) {
		qDebug() << "FTP: Connecting to " << urlpath;
		ftp->connectToHost(urlpath, 21);
		qDebug() << "FTP: Logging...";
		ftp->login();
	} else {
		ftp->connectToHost(url.host(), url.port(21));
		if (!url.userName().isEmpty())
			ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
		else
			ftp->login();
		if (!url.path().isEmpty())
			ftp->cd(url.path());
	}
	// wait
	for (int i = 0; (i < timeout*10) || (!connected); i++)
		QTest::qWait(100);
	return connected;
}

void	Ftp::Disconnect(void) {
	if (ftp) {
		ftp->abort();
		ftp->deleteLater();
		ftp = 0;
		connected = false;
	}
	return;
}

bool	Ftp::Cd(const QString & dir)
{
	ready = false;
	ftp->cd(dir);
	return wait4ready();
}

bool	Ftp::List(const QString & dir)
{
	ready = false;
	ftp->list(dir);
	return wait4ready();
}

bool	Ftp::MkDir(const QString & dir)
{
	ready = false;
	ftp->mkdir(dir);
	return wait4ready();
}

bool	Ftp::RmDir(const QString & dir)
{
	ready = false;
	ftp->list(dir);
	return wait4ready();
}

bool	Ftp::Get(const QString & fileName)
{
	if (QFile::exists(fileName)) {
		qDebug() << tr("There already exists a file called %1 in the current directory.").arg(fileName);
		return false;
	}
	file = new QFile(fileName);
	if (!file->open(QIODevice::WriteOnly)) {
		qDebug() << tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString());
		delete file;
		return false;
	}
	ftp->get(fileName, file);
	return true;
}

bool	Ftp::Rename(const QString & oldfile, const QString & newfile)
{
	ready = false;
	ftp->rename(oldfile, newfile);
	return wait4ready();
}

bool	Ftp::Remove(const QString & file)
{
	ready = false;
	ftp->remove(file);
	return wait4ready();
}

// private slots
void	Ftp::ftpCommandFinished(int, bool error)
{
	//qDebug() << "FTP: command finnished"; 
	if (ftp->currentCommand() == QFtp::ConnectToHost) {
		if (error) {
			qDebug() << tr("Unable to connect to the FTP server at %1. Please check that the host name is correct.").arg(currentHost);
			Disconnect();
			return;
		}
		qDebug() << "FTP: Connected ok";
		return;
	}
	if (ftp->currentCommand() == QFtp::Login) {
		if (error) {
			qDebug() << tr("Unable to log in to the FTP server at %1. Please check that the login/password is correct.").arg(currentHost);
			Disconnect();
			return;
		}
		qDebug() << "FTP: Logged ok";
		connected = true;
	}
	if (ftp->currentCommand() == QFtp::List) {
		if (error) {
			qDebug() << tr("Unable to list FTP server at %1. Please check that the URL is correct.").arg(currentHost);
			return;
		}
		qDebug() << "FTP: Listed ok";
		ready = true;
	}
	if (ftp->currentCommand() == QFtp::Get) {
		if (error) {
			file->close();
			file->remove();
		} else
			file->close();
		delete file;
	}
}

void	Ftp::ftpStateChanged(int state) { qDebug() << "FTP: state changed:" << state; }

void	Ftp::addToList(const QUrlInfo &urlInfo)
{
	fileList[urlInfo.name()] = urlInfo;	// name(), size(), owner(), group(), lastModified(), isDir()
	//qDebug() << urlInfo.name();
}

void	Ftp::enableConnect()
{
	// Save the used configuration
	QNetworkConfiguration config = networkSession->configuration();
	QString id;
	if (config.type() == QNetworkConfiguration::UserChoice)
		id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
	else
		id = config.identifier();
	QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
	settings.beginGroup(QLatin1String("QtNetwork"));
	settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
	settings.endGroup();
	qDebug() << "FTP: Connection opened";
	connected = networkSession->isOpen();
}

// private

bool	Ftp::wait4ready(const int timeout) {
	// timeout - in sec
	for (int i = 0; (i < (timeout * 10)) || (!ready); i++)
		QTest::qWait(100);
	return ready;
}
