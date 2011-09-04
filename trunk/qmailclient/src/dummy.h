#ifndef __DUMMY_H__
#define __DUMMY_H__

#include <QApplication>
#include <QByteArray>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QImage>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QtCore>
#include <QTextStream>
#include <QThread>
#include <QtNetwork>

class QTextStream;
class QHostInfo;

class Mime
{
public:
	Mime() {};
	static QString decodeQP(QString s) {}; //decodes a string to quoted-printable
	static QString encodeQP(QString s) {}; //encodes a string to quoted-printable
};

class MimeTypes
{
	QHash<QString,QString> mimeTypes;

public:
	MimeTypes() {};
	QString value(QString key){return mimeTypes.value(key);};
	void insert(QString key, QString value){};
};

class Mail;

class MailPrivate
{
public:
	MailPrivate(Mail *_owner, Mail *_parent) {};
	MailPrivate (const MailPrivate &copy) {};
	~MailPrivate () {};
/*--------------------------------------------------------------------------*/
	Mail* owner;
	Mail* parent;
	QString text_body;
	QList<Mail *> mail_parts;
	QMap<QString, QString> header;
	QString send_data;
	QByteArray raw_data;
	bool is_parsed;
	bool is_singlePart;
	uint index;
	bool is_send_data_valid;
	QString mime_source;
	QTemporaryFile *temp_file;
	QString temp_dir;
};

class Mail : public QObject
{
	Q_OBJECT
public:
	Mail(Mail* parent = 0) {};
	Mail (const Mail &copyMe) {};
	~Mail() {};
	QString messageBody() {};
	QString textBody(){return d->text_body;};
	void setMessageBody(const QString& body) {};
	QString encoding() {};
	int setEncoding(const QString& encod) {};
	void setContentType(QString typ) {};
	void setContentTransferEncoding(QString enc) {};
	QMap<QString, QString> header(){return d->header;};
	QString header(QString key){return d->header.value(key);};
	void setHeader(QString from, QString to, QString cc, QString bcc, QString subject) {};
	void setHeader(QString key, QString value) {};
	QString from(){return d->header.value("From");};
	QString to(){return d->header.value("To");};
	int emptyBcc(){return d->header.remove("Bcc");};
	QString cc(){return d->header.value("Cc");};
	QString bcc(){return d->header.value("Bcc");};
	QString subject(){return d->header.value("Subject");};
	QString getFileName() {};
	uint longestLine(const QString cstr) {};
	QString breakLongLines(const QString& in, uint max_len, QString separator="=") {};
	void attachFile(QString fileName) {};
	void attachMail(Mail *M) {};
	bool isSinglePart() {};
	bool isParsed() {};
	QString getBodySendDataForText() {};
	void setData(const QString& mail_data) {};
	void parseData(const QString& ts) {};
	void parseBody(QByteArray cstr_body) {};
	uint parseHeader(QByteArray data) {};
	void parseHeaderLine(QString line) {};
	enum Format { FullMessage, DataToSign };
	QString getSendData (Format f = FullMessage) {};
	QString getHeaderData() {};
	void setSendData(const QString& send_data) {};
	void buildSendData (Format f = FullMessage) {};
	QString getBodySendData() {};
	uint getPartCount() {};
	Mail* getPart(uint index) {};
	uint addPart(Mail* mp) {};
	int findPart(Mail* mp) {};
	bool removePart (Mail* mp) {};
	QByteArray getRawData() {};
	void setRawData(const QByteArray& data) {};
	bool isText() {};
	bool testBoundary(const QString& bound) {};
	QString getAddress() {};
	Mail* getFromAddress(Mail* parent, const QString& address) {};
	QString getShowText() {};
	QString contentType() {};
	uint getMemSize() {};
	void cleanTemp() {};
	QString saveInTemp() {};
	QString makeMimeSourceImg(QImage imp) {};
	QString timeStamp() {};
protected:
	virtual bool isSendDataValid() {};
	virtual void setSendDataValid(bool VAL = TRUE) {};
private:
	MailPrivate* d;
	MimeTypes mimeTypes;
};

class Pop3Private;

class Pop3 : public QObject {
	Q_OBJECT
public:
	Pop3 (const QString &user,const QString &passwd, const QString &pop3_server,QObject *parent=0) {};
	~Pop3() {};
	void getAllMails(bool del=false) {};
	QString getPop3Server () const;
	QString getUser () const;
	QString getPassword () const;
signals:
	void status( const QString & );
	void error( const QString & );
	void disconnected(Pop3*);
	void newMail(Mail*);
private slots:
	void dnsLookupHelper(const QHostInfo &hostInfo) {};
	void readyRead() {};
	void connected() {};
	void connectionClosed() {};
private:
	void openConnection() {};
	void quit() {};
	void parseStatLine(const QString& line) {};
	void parseMail(const QString& mail) {};
	void nextLine(const QString& line) {};
	Pop3Private* d;
};

enum State {
};

class Smtp : public QThread
{
	Q_OBJECT
public:
	Smtp(const QString &lsmtpusername, const QString &lsmtppass, const QString &lsmtphost, QObject *parent) {};
	~Smtp() {};
	QStringList ErrorMSG;
	void setPreserveMails(bool p){};
	void queueMail(Mail *m, bool qas=false) {};
	void sendMail(Mail *m, bool qas=false) {};
	void disconnectSmtp() {};
	bool error;
	bool isRunning(){return running;};
	int sentMails(){return mailsSent;};
	int mailsToSend(){return queuedMails.size();};
protected:
	void run() {};
signals:
	void status( const QString &);
	void ConnectorSuccess();
	void connected();
	void sendNextLine();
	void ErrorCloseAll();
	void SuccessQuit();
	void ReadyToSend();
	void mailSent(Mail *m);
private slots:
	void grabLine() {};
	void exitLoop() {};
	void ReadLiner() {};
	void nextLine() {};
	void sendLine(QString senddata) {};
private:
	bool preserveMails;
	QList<Mail *> queuedMails;
	int mailsSent;
	bool running;
};
#endif // __DUMMY_H__
