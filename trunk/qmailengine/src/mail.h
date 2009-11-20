/****************************************************************************
* http://sourceforge.net/projects/nlcreator/
*
* QNewsletterCreator - Business Email Client for Mass Mails
* Nuntius Leo - Personal Qt Email Client
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License Version 2 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
* USA. http://www.gnu.org/copyleft/library.html
*
* Please report all bugs and problems to the project admins:
* http://sourceforge.net/projects/nlcreator/
* 
*
* Copyright by dontinelli@users.sourceforge.net if no additional copyright information is given
* otherwise copyright by given authors/organizations 
* and modfified, e.g. ported, by dontinelli@users.sourceforge.net.
* Some code has been taken from 
* http://sourceforge.net/projects/lhmail - License : GNU Library General Public License    
* Authors: lukasz.iwaszkiewicz@gmail.com
* lukasz.iwaszkiewicz@lefthand.com.pl, lukasz.iwaszkiewicz@software.com.pl 
* Copyright (C) 2004/2005 LeftHand sp.z.o.o. info@lefthand.com.pl 
*
*****************************************************************************/

#ifndef MAIL_H
#define MAIL_H

#include <QObject>
#include <QDebug>
#include <QtCore>
#include <QImage>
#include <QMessageBox>

#include "mime.h"

class Mail;

#ifndef MP_CONST 
#define MP_CONST 

#define MP_CONVERT_CRLF 0x0001
#define MAX_LINE_LEN 75

#endif

#ifdef Q_OS_WIN32
 __declspec(dllexport)
#endif

class MailPrivate {
public:
	MailPrivate(Mail *_owner, Mail *_parent) :
	owner(_owner),
	parent(_parent),
	text_body(),
	mail_parts(),
	header(),
	send_data(),
	raw_data(),
	is_parsed( FALSE ),
	index(0),
	is_send_data_valid( FALSE ),
	mime_source(),
	temp_file() {
		init ();
	};

/*--------------------------------------------------------------------------*/
	MailPrivate (const MailPrivate &copy);

/*--------------------------------------------------------------------------*/

	void init () {
		//QString encoding = "ISO 8859-1"; //ISO 8859-1
		//QTextCodec* cod = QTextCodec::codecForLocale();
		//if (cod != NULL)
		//{
		//	encoding = cod->name();
		//}

		header.insert("MIME-VERSION","1.0");
		//header.insert("Content-Type","text/plain");
		//header.insert("charset", encoding);
		//header.insert("Content-Transfer-Encoding", "quoted-printable");

		is_singlePart=true;

		//mail_parts.setAutoDelete(true);
	}

/*--------------------------------------------------------------------------*/

	Mail* owner;
	/**
	 * Mail or MailPart that contains this part, NULL for mails
	 */ 
	Mail* parent; 
	QString text_body;
	QList<Mail *> mail_parts;
	QMap<QString, QString> header;
	/**
	 * Text that is sent by SMTP, or received with header
	 */ 
	QString send_data;
	/**
	 * Content of attached filed
	 */ 
	QByteArray raw_data;

	bool is_parsed;
	bool is_singlePart;
	
	uint index;
	
	bool is_send_data_valid;
	
	QString mime_source;
	QString temp_file;
};

/****************************************************************************/

class Mail : public QObject
{
    Q_OBJECT

public:
		
	/**
	 * Constructor
	 * @param parent For LHMailPart LHMail conatining this part
	 */ 
	Mail(Mail* parent = 0);
	Mail (const Mail &copyMe);

	~Mail();
	
	/**
	 * @return Text representing message body
	 */
	QString messageBody();
	QString textBody(){return d->text_body;};
	
	/**
	 * Sets message body
	 */
	void setMessageBody(const QString& body);
	
	/**
	 * @return encoding of mail or mail part (like ISO-8859-2)
	 */
	QString encoding();
	
    /**
	 * Used to set encoding of mail (like ISO-8859-2). 
	 * On Unix you generally do not have to set encoding explicitly --
	 * encoding for Locale is used. 
	 * <br>
	 * On Windows machines you should set this.
	 * @param encod String representing encoding like ISO-8859-2
	 */
	int setEncoding(const QString& encod);
	void setContentType(QString typ);
	void setContentTransferEncoding(QString enc);
 
	/**
	 * @return @ref LHMailHeader of this mail
	 */
	QMap<QString, QString> header(){return d->header;};
	
	QString header(QString key){return d->header.value(key);};
	
	void setHeader(QString from, QString to, QString cc, QString bcc, QString subject);
	
	void setHeader(QString key, QString value);
	
	QString from(){
		return d->header.value("From");
	};

	QString to(){
		return d->header.value("To");
	};
	
	int emptyBcc(){
		return d->header.remove("Bcc");
	};
	
	QString cc(){
		return d->header.value("Cc");
	};

	QString bcc(){
		return d->header.value("Bcc");
	};
	
	QString subject(){
		return d->header.value("Subject");
	};
	
	QString getFileName();
	
	//two helper-function
	uint longestLine(const QString cstr);
	QString breakLongLines(const QString& in, uint max_len, QString separator="=");

	/**
	 * Used to check if mail or mail part is single or multipart.
	 * @return True if mail is not multipart
	 */
	bool isSinglePart();
	
	/**
	 * @return True if mail or mail part was received and parsed
	 */ 
	bool isParsed();

	/**
	 * @return Encoded @ref messageBody ready for sending
	 */ 
	QString getBodySendDataForText();
	
	/**
	 * Used to set data for received mails
	 * Next data is parsed devided to header and body
	 * This data is converted according to setting in included headers
	 */ 
	void setData(const QString& mail_data);
	
	/**
	 * Parses mail data set by @ref setData
	 * First calls parse for @ref LHMailHeader
	 * then parses body @ref parseBody
	 */ 
	void parseData(const QString& ts);
	
	/**
	 * Parses body of mail or mail part
	 */ 
	void parseBody(QByteArray cstr_body);
	
	uint parseHeader(QByteArray data);
	
	void parseHeaderLine(QString line);

	/**
	*  Format of data to be built by buildSendData, and returned by getSendData.
	*/
	enum Format { FullMessage, DataToSign };
	
	/**
	 * Returnes data to be send or received data,
	 * If send data is not valid @ref buildSendData
	 * is called first
	 */ 
	QString getSendData (Format f = FullMessage);
	
	QString getHeaderData();

	/**
	 * Used to set send data, you should not change this method, rather
	 * @ref buildSendData, or @ref getBodySendData
	 */ 
	void setSendData(const QString& send_data);
        
	/**
	 * Method called just before sending data
	 * In implementation send data is prepared
	 */ 
	void buildSendData (Format f = FullMessage);
	
	/**
	 * @return data to be send without header
	 */ 
	QString getBodySendData();
	
	/**
	 * @return Number of parts
	 */ 
	uint getPartCount();
	
	/**
	 * @return Part of given index, or NULL id index is out of range
	 */ 
	Mail* getPart(uint index);
	
	/**
	 * Adds mail part to mail or mail part. Do not use this method
	 * directly. Use @ref LHMail::attachFile instead.
	 * @return Part count
	 */ 
	uint addPart(Mail* mp);
	
	/**
	 * Looks for position of part in part collection
	 * @return Index in collection or -1 if not found
	 */ 
	int findPart(Mail* mp);
	
	/**
	 * Removes mail part to mail or mail part. Do not use this method
	 * directly. Use @ref LHMail::removeAttachment instead.
         * If mp is set to NULL, all parts will be removed.
	 * @return true if removed false if not found
	 */ 
	bool removePart (Mail* mp);
	
	/**
	 * @return Raw data for attachment, set or received
	 */ 
	QByteArray getRawData();
	
	/**
	 * Used to set raw data for attachment. Do not use directly.
	 */ 
	void setRawData(const QByteArray& data);

	/**
	 * @return True if mail contains text, ie. if Content-Type is text
	 */ 
	bool isText();
	
	/**
	 * @return True if this boundary can be used for this mail or mail part
	 */ 
	bool testBoundary(const QString& bound);
	
	/**
	 * @return Address of mail or mailpart in part tree. This address is unique in mail
	 * And can be used to find this part or mail using @ref getFromAddress
	 */ 
	QString getAddress();

	/**
	 * Method used to find mail or part in mail using address rceived from @ref getAddress.
	 * @param parent The top most mail
	 * @param address String obtainde by @ref getAddress
	 * @return Mail o part if success NULL otherwise
	 */ 
	Mail* getFromAddress(Mail* parent, const QString& address);
	
	/**
	 * Method returning HTML that is showed to user.
	 */ 
	 QString getShowText();
	
	/**
	 * @return Value of Content-Type header
	 */ 
	QString contentType();
	
	/**
	 * @return Approximate of used memory by this mail or part
	 */ 
	uint getMemSize();
	
	/**
	 * Cleans temp files and memory used by this mail or part
	 */ 
	void cleanTemp();
	
	/**
	 * Save content of part in temp file. You can clean this using @ref cleanTemp.
	 * Original file name is used and is put into unique directory.
	 * @return Full path to temp file.
	 */ 
	QString saveInTemp();
	
	/**
	 * Used by parts containing graphics to create temporary mime source
	 * @return Name of mime source (unique in mail)
	 */ 
	QString makeMimeSourceImg(QImage imp);
	
	/**
	* Return the current time in right format to add to header.
	* Function is called, when building header data.
	*/
	QString timeStamp();
	
protected:
	
	virtual bool isSendDataValid();
	
	virtual void setSendDataValid(bool VAL = TRUE);
		
private:

	MailPrivate* d;
};

#endif
