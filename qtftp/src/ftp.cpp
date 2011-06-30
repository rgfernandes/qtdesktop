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

void Ftp::Connect(const QString &urlpath)
{
	// if connectEnabled...
	currentHost = urlpath;
	ftp = new QFtp(this);
	connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCommandFinished(int,bool)));
	connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(addToList(QUrlInfo)));
	//connect(ftp, SIGNAL(stateChanged(int)), this, SLOT(ftpStateChanged(int)));
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
	connected = false;
	QTest::qWait(100);
}

void Ftp::Disconnect(void) {
	if (ftp) {
		ftp->abort();
		ftp->deleteLater();
		ftp = 0;
		connected = false;
	}
	return;
}

//void Ftp::ftpStateChanged(int state) { qDebug() << "FTP: state changed:" << state; }
//void Ftp::ftpCommandStarted(int id) { qDebug() << "FTP: command started: " << id; }
//void Ftp::ftpDone(int err) { qDebug() << "FTP: done: " << err; }

void Ftp::ftpCommandFinished(int, bool error)
{
	qDebug() << "FTP: command finnished"; 
	if (ftp->currentCommand() == QFtp::ConnectToHost) {
		if (error) {
			qDebug() << tr("Unable to connect to the FTP server at %1. Please check that the host name is correct.").arg(currentHost);
			Disconnect();
			return;
		}
		qDebug() << "FTP: Connected ok";
		connected = true;
		return;
	}
	if (ftp->currentCommand() == QFtp::Login) {
		qDebug() << "FTP: Logged ok";
		ftp->list();
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

void Ftp::addToList(const QUrlInfo &urlInfo)
{
	fileList[urlInfo.name()] = urlInfo;	// name(), size(), owner(), group(), lastModified(), isDir()
}

void Ftp::enableConnect()
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

void Ftp::FileGet(QString & fileName)
{
	if (QFile::exists(fileName)) {
		qDebug() << tr("There already exists a file called %1 in the current directory.").arg(fileName);
		return;
	}
	file = new QFile(fileName);
	if (!file->open(QIODevice::WriteOnly)) {
		qDebug() << tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString());
		delete file;
		return;
	}
	ftp->get(fileName, file);
}

void Ftp::DirCd(QString & name)
{
	if (name == "..") {
		if (!currentPath.isEmpty()) {
			fileList.clear();
			currentPath = currentPath.left(currentPath.lastIndexOf('/'));
			if (currentPath.isEmpty()) {
				ftp->cd("/");
			} else
				ftp->cd(currentPath);
			ftp->list();
		}
	} else {
		if (fileList[name].isDir()) {
			fileList.clear();
			currentPath += '/';
			currentPath += name;
			ftp->cd(name);
			ftp->list();
		}
	}
	return;
}
