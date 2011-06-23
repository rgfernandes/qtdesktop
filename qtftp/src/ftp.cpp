#include <QtGui>
#include <QtNetwork>

#include "ftp.h"

Ftp::Ftp(void) : ftp(0) {
	connect(fileList, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(processItem(QTreeWidgetItem*,int)));
	connect(fileList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(enableDownload()));
	connect(connectButton, SIGNAL(clicked()), this, SLOT(connectOrDisconnect()));
	connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadFile()));
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
		connect(networkSession, SIGNAL(opened()), this, SLOT(enableConnectButton()));
		connectEnabled = false;
		statusLabel->setText(tr("Opening network session."));
		networkSession->open();
	}
}

void Ftp::connectOrDisconnect()
{
	if (ftp) {
		ftp->abort();
		ftp->deleteLater();
		ftp = 0;
		cdToParentEnabled = false;
		downloadEnabled = false;
		connectEnabled = true;
		return;
	}
	ftp = new QFtp(this);
	connect(ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpCommandFinished(int,bool)));
	connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(addToList(QUrlInfo)));
	fileList->clear();
	currentPath.clear();
	isDirectory.clear();
	QUrl url(ftpServerLineEdit->text());
	if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) {
		ftp->connectToHost(ftpServerLineEdit->text(), 21);
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
	fileList->setEnabled(true);
	connectEnabled = false;
}

void Ftp::downloadFile()
{
	QString fileName = fileList->currentItem()->text(0);
	if (QFile::exists(fileName)) {
		QMessageBox::information(this, tr("FTP"),tr("There already exists a file called %1 in the current directory.").arg(fileName));
		return;
	}
	file = new QFile(fileName);
	if (!file->open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("FTP"), tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString()));
		delete file;
		return;
	}
	ftp->get(fileList->currentItem()->text(0), file);
}

void Ftp::ftpCommandFinished(int, bool error)
{
	setCursor(Qt::ArrowCursor);
	if (ftp->currentCommand() == QFtp::ConnectToHost) {
		if (error) {
			QMessageBox::information(this, tr("FTP"),tr("Unable to connect to the FTP server at %1. Please check that the host name is correct.").arg(ftpServerLineEdit->text()));
			connectOrDisconnect();
			return;
		}
		statusLabel->setText(tr("Logged onto %1.").arg(ftpServerLineEdit->text()));
		fileList->setFocus();
		downloadEnabled = true;
		connectEnabled = true;
		return;
	}
	if (ftp->currentCommand() == QFtp::Login)
		ftp->list();
	if (ftp->currentCommand() == QFtp::Get) {
		if (error) {
			statusLabel->setText(tr("Canceled download of %1.").arg(file->fileName()));
			file->close();
			file->remove();
		} else
			file->close();
		delete file;
		enableDownload();
	} else if (ftp->currentCommand() == QFtp::List) {
		if (isDirectory.isEmpty()) {
			fileList->addTopLevelItem(new QTreeWidgetItem(QStringList() << tr("<empty>")));
			fileList->setEnabled(false);
		}
	}
}

void Ftp::addToList(const QUrlInfo &urlInfo)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setText(0, urlInfo.name());
	item->setText(1, QString::number(urlInfo.size()));
	item->setText(2, urlInfo.owner());
	item->setText(3, urlInfo.group());
	item->setText(4, urlInfo.lastModified().toString("MMM dd yyyy"));
	QPixmap pixmap(urlInfo.isDir() ? ":/images/dir.png" : ":/images/file.png");
	item->setIcon(0, pixmap);
	isDirectory[urlInfo.name()] = urlInfo.isDir();
	fileList->addTopLevelItem(item);
	if (!fileList->currentItem()) {
		fileList->setCurrentItem(fileList->topLevelItem(0));
		fileList->setEnabled(true);
	}
}

void Ftp::processItem(QTreeWidgetItem *item, int /*column*/)
{
	QString name = item->text(0);
	if (isDirectory.value(name)) {
		fileList->clear();
		isDirectory.clear();
		currentPath += '/';
		currentPath += name;
		ftp->cd(name);
		ftp->list();
		cdToParentEnabled = true;
		return;
	}
}

void Ftp::cdToParent()
{
	setCursor(Qt::WaitCursor);
	fileList->clear();
	isDirectory.clear();
	currentPath = currentPath.left(currentPath.lastIndexOf('/'));
	if (currentPath.isEmpty()) {
		cdToParentEnabled = false;
		ftp->cd("/");
	} else
		ftp->cd(currentPath);
	ftp->list();
}

void Ftp::enableDownload()
{
	QTreeWidgetItem *current = fileList->currentItem();
	if (current) {
		QString currentFile = current->text(0);
		downloadEnabled = !isDirectory.value(currentFile);
	} else
		downloadEnabled = false;
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
	connectEnabled = networkSession->isOpen();
}
