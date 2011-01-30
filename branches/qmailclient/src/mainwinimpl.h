#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include <QtGui>
#include <QtSql>

#include "ui_MainWin.h"
#include "mbtreemodel.h"

#include "dummy.h"
//#include "mailbox.h"

//MailTreeWidgetItem is used to enable adding Mails to a TreeWidget
class MailTreeWidgetItem : public QTreeWidgetItem {
public:
	MailTreeWidgetItem(QTreeWidgetItem *parent, Mail *mp):QTreeWidgetItem(parent,QStringList()<<mp->header("Subject") 
		<<mp->header("Content-Type")<<mp->header("Content-Transfer-Encoding")<<QString("%1 B").arg(mp->getMemSize()),QTreeWidgetItem::UserType) {
		m=mp;
	};
	MailTreeWidgetItem(QTreeWidget *parent, Mail *mp):QTreeWidgetItem(parent,QStringList()<<mp->header("Subject") 
		<<mp->header("Content-Type")<<mp->header("Content-Transfer-Encoding")<<QString("%1 B").arg(mp->getMemSize()),QTreeWidgetItem::UserType) {
		m=mp;
	};
	Mail* getMail(){return m;};
private:
	Mail *m;
};

//MailTreeWidgetItem is used to enable adding Mails to a TreeWidget
class DateTableWidgetItem : public QTableWidgetItem
{
protected:
	inline bool operator<( const QTableWidgetItem & other )const {
		const DateTableWidgetItem *o = dynamic_cast<const DateTableWidgetItem*>(&other);
		qDebug()<<this->d<<o->d;
		if (this->d < o->d) return true;
		return false;
	}; 
public:
	DateTableWidgetItem(const QString &text):QTableWidgetItem(text) {
		QLocale e=QLocale(QLocale::English,QLocale::UnitedKingdom);
		qDebug()<<text.left(25);
		d=e.toDateTime(text.left(25),"ddd, dd MMM yyyy hh:mm:ss");
		qDebug()<<d;
	};
	DateTableWidgetItem(const QDateTime &date):QTableWidgetItem(date.toString()) {d=date;};
private:
	QDateTime d;
};

class MainWinImpl : public QMainWindow, public Ui::MainWin {
Q_OBJECT
public:
	MainWinImpl( QSqlDatabase *d, QWidget * parent = 0, Qt::WFlags f = 0 );
	~MainWinImpl(){
		//delete db;		
		delete p;
		delete smtp;
	};
	void	setModels(QSqlDatabase *);
private slots:
	void addMailPartsToList(QTreeWidgetItem*parent, Mail *m); //add a mailpart to the overview of all parts of a mail
	void addMailToList(Mail *m,QString fileName); //add mail to the list of mails
	void downloadMails();
	void mailSent(Mail *mail); //this slot is used to take handle of a mail after the sending process
	void newMail(Mail *mail);	//this slot is used to take handle of newly received mails
	void displayState(QString s); //display message in the statebar
	void loadMails(QString path); //load local mails i the list of mails; for each mail in a folder addMailToList is called
	void on_actionAccountAdd_triggered();
	void on_actionAccountEdit_triggered();
	void on_actionAccountDel_triggered();
	void on_actionDeleteMail_triggered();
	void on_actionEditMail_triggered();
	void on_actionExit_triggered(){close();};
	void on_actionForwardMail_triggered();
	void on_actionNewMail_triggered();
	void on_actionReplyAll_triggered();
	void on_actionReplyMail_triggered();
	void on_dirTree_clicked(const QModelIndex &index);
	void on_lstMails_cellClicked(int curRow, int curCol);
	void on_mailBrowser_anchorClicked( const QUrl &url); //slot to handle clicks on attachment-links
	void on_mailParts_currentItemChanged( QTreeWidgetItem * current, QTreeWidgetItem * previous );
	void sendMail(Mail *m); //send a mail
	
private:
	QSqlDatabase		*db;
	QSqlTableModel		*modelA;
	// not mine
	QString generateMailName(QString dir=""); //routine to generate a unique name for a mail, dir=path to file
	void iniMailEngine();
	void iniMailFolders();
	void readSettings();

	MBTreeModel *mbTreeModel;
	QDirModel *dirModel;
	QString mailBoxRootPath;
	QDir inboxDir;
	QDir outboxDir;
	QDir trashDir;
	QDir draftDir;
	
	Pop3 *p;
	Smtp *smtp;
	Mail *currentMail;
	
	//settings
	QString username;
	QString mailAddress;
	QString popHost;
	QString popUsr;
	QString popPass;
	QString smtpHost;
	QString smtpUsr;
	QString smtpPass;
};
#endif
