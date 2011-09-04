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

#include <iostream>
#include <QMapIterator>

#include "mail.h"

MailPrivate::MailPrivate(const MailPrivate &copy)
{
        //??? Tak sie nie da.... No wiec jak?
        /*
        for (QPtrList <Mail>::const_iterator i = copy.mail_parts.begin (); i != copy.mail_parts.end (); i++)
                mail_parts.append (new Mail (*i));
        */
        
        // ?
        owner =               copy.owner;
        parent =              copy.parent;

        header =              copy.header;
        text_body =           copy.text_body;
        send_data =           copy.send_data;
        raw_data =            copy.raw_data;
        is_parsed =           copy.is_parsed;
        index =               copy.index;
        is_send_data_valid =  copy.is_send_data_valid;
        mime_source =         copy.mime_source;
        temp_file =           copy.temp_file;

        init ();
}

/****************************************************************************/

Mail::Mail (Mail* parent)
{
	d = new MailPrivate (this, parent);
}

/****************************************************************************/

Mail::Mail(const Mail &copyMe) : QObject()
{
	d = new MailPrivate(*copyMe.d);
}

/****************************************************************************/

Mail::~Mail()
{
	delete d;
}

/****************************************************************************/

QString Mail::messageBody()
{
	
	if (d->is_singlePart)
		return d->text_body;
	else {
		QString ret;
		int pc = getPartCount();
		
		for (uint i = 0; i < pc; i++)
		{
			Mail* mp = getPart(i);
			if (mp->header().value("Content-Type")=="text/plain") 
				ret += mp->messageBody();
		}
		//qDebug()<<ret;
		return ret;
	}
}

/****************************************************************************/

void Mail::setMessageBody(const QString& body)
{
	setSendDataValid( FALSE );
	
	d->text_body = body;
};

void Mail::setHeader(QString from, QString to, QString cc, QString bcc, QString subject)
{
	if(from!="") d->header.insert("From", from);
	if(to!="") d->header.insert("To",to);
	if(cc!="") d->header.insert("Cc",cc);
	if(bcc!="") d->header.insert("Bcc",bcc);
	if(subject!="") d->header.insert("Subject",subject);
}

void Mail::setHeader(QString key, QString value)
{
	if(key=="boundary"){
		int i=1;
		while(!testBoundary(value)){
			value.replace(value.size()-1,1,QChar(i+33));
			i++;
		}
	}
	d->header.insert(key, value);
}

bool Mail::isSinglePart()
{
	return d->is_singlePart;
}

QString Mail::encoding()
{
	return d->header.value("charset");
};

int Mail::setEncoding(const QString& encod)
{
	setSendDataValid( FALSE );
	d->header.insert("charset", encod);
	return 0;
}

void Mail::setContentType(QString typ)
{
	setSendDataValid( FALSE );
	//qDebug()<<d->header.value("Content-Type");
	d->header.insert("Content-Type", typ);
	//qDebug()<<d->header.value("Content-Type");
}

void Mail::setContentTransferEncoding(QString enc)
{
	setSendDataValid( FALSE );
	d->header.insert("Content-Transfer-Encoding", enc);
}

QString Mail::getFileName()
{
	QString file_name = d->header.value("Content-Type", "filename");
	if (file_name.isEmpty())
	{
		file_name = d->header.value("Content-Type", "name");
	}
	if (file_name.isEmpty())
	{
		file_name = d->header.value("Content-Disposition", "filename");
	}
	if (file_name.isEmpty())
	{
		file_name = "no_name";
	}
	
	QFileInfo fi(file_name);
	file_name = fi.fileName();
	
	return file_name;
}

bool Mail::testBoundary(const QString& bound)
{
	QString sd = getSendData();
	if (sd.contains(bound))
	{
		return false;
	}
	
	uint pc = getPartCount();
	
	for (uint i=0; i<pc; i++)
	{
		if (!getPart(i)->testBoundary(bound))
		{
			return false;
		}
	}
	
	return true;
}

bool Mail::isParsed()
{
	return d->is_parsed;
}

bool Mail::isText()
{
	QString cnt_type = d->header.value("Content-Type");	
	if (cnt_type.contains("text")) return true;

	return false;
}

void Mail::attachFile(QString fileName)
{
qDebug()<<"attach file"<<fileName;
	Mail *mp=new Mail(this);
	QFile f(fileName);
	f.open(QIODevice::ReadOnly);
	QByteArray ba=f.readAll();
	//qDebug()<<"size"<<ba.size();
	f.close();
	
	QString ct,name;
	name=f.fileName().right(f.fileName().size()-f.fileName().lastIndexOf("/")-1);
	ct=mimeTypes.value(name.right(name.size()-name.lastIndexOf(".")-1));

	if(ct=="")ct="application/x-unknown";
	
	mp->setHeader("Content-Type",QString("%1; \r\n name=\"%2\"").arg(ct).arg(name));
	mp->setContentTransferEncoding("base64");
	mp->setHeader("Content-Disposition","attachment;\r\n filename="+name);
	mp->setRawData(ba);
	addPart(mp);
}

void Mail::attachMail(Mail *m)
{
	QString sendData=m->getSendData();
	
	qDebug()<<sendData;
	
	Mail *mp=new Mail(this);
	mp->setHeader("Content-Disposition", "inline");
	mp->setHeader("Content-Type", "message/rfc822");
	mp->setHeader("Content-Description", m->subject());
	mp->setMessageBody(sendData);
	addPart(mp);
	delete m;
}

/**
* helper function returning length of the longest line ended by \r\n
*/
uint Mail::longestLine(const QString cstr)
{
	int start_line = 0;
	int end_line = 0;
	uint ret = 0;
	while (end_line >= 0)
	{
		uint llen = 0;
		end_line = cstr.indexOf("\r\n", start_line);
		
		if (end_line <0 )
		{
			llen = cstr.length() - start_line;
		}
		else
		{
			llen = end_line - start_line;
			start_line = end_line+1;
		};
		
		if (llen>ret)
		{
			ret=llen;
		};
	}
	
	return ret;
}

QString Mail::breakLongLines(const QString& in, uint max_len, QString separator)
{
	
	QStringList strings=in.split("\r\n");
	
	for(int i=0;i<strings.count();i++){
		if(strings[i].size()>max_len){
			//line is too long, strip
			int len=max_len-1;
			if(strings[i].at(max_len-1)=='=') len--;
			else if(strings[i].at(max_len-2)=='=') len-=2;
			else if(strings[i].at(max_len-3)=='=') len-=3;
			QString left = strings[i].left(len)+separator;
			QString right = strings[i].right(strings[i].size()-len);
			strings[i]=left;
			strings.insert(i+1,right);
		}
	};
	
	QString ret=strings.join("\r\n");
	return ret;
	
	/*
	//this is the old code from lhmail:
	
	uint in_size = in.length();
	uint out_size = in_size + (in_size/max_len)*3 +1;
	
	const QChar *line_sep("=\r\n");
	
	ret.resize(out_size);
	ret.fill(0, out_size);
	
	QChar* c_ret = ret.data();
	const QChar* c_in = &in;
		
	int start_line = 0;
	int end_line = 0;
	
	int out_off = 0;
	
	uint start = 0;
	uint step = 0;
	
	bool more = true;
	
	while (more)
	{
		uint llen = 0;
		end_line = in.indexOf("\r\n", start_line);
		if (end_line <0 )
		{
			more = false;
			end_line = in_size;
		}
		else
		{
			end_line += 2;
		};
		
		llen = end_line - start_line;
		
		if (llen>max_len)
		{
			//line is too long
			start = start_line;
			step = max_len;
			while (start+step < end_line)
			{
				qDebug("breakLongLines, piece of long line, out_off: %d, start: %d, step: %d", out_off, start, step);
				if (*(c_in + start + step -1) == '=')
				{
					step--;
				}
				if (*(c_in + start + step -2) == '=')
				{
					step-=2;
				}
				qmemmove(c_ret + out_off, c_in + start, step);
				out_off += step;
				start += step;
				qmemmove(c_ret + out_off, line_sep, 3);
				out_off += 3;
			};
			if (end_line >= 0)
			{
				llen += 2;
			};
			step = end_line - start;
			qDebug("breakLongLines, last piece of long line, out_off: %d, start: %d, llen: %d, step: %d", out_off, start, llen, step);
			qmemmove(c_ret + out_off, c_in + start, step);
			out_off += step;
		}
		else
		{
			//line is short so we copy it with \r\n on the end
			qmemmove(c_ret + out_off, c_in + start_line, llen);
			out_off += llen;
		};
		
		start_line = end_line;
	};
	*/
	return ret;
}

/****************************************************************************/

QString Mail::getBodySendData()
{
	QByteArray send_body;
	
	send_body=d->raw_data;
	
	if (d->header.value("Content-Transfer-Encoding")=="base64") send_body=send_body.toBase64();
	
	return QString::fromAscii(send_body);
}

QString Mail::getBodySendDataForText()
{
	QString send_body;
	
	//QTextCodec *codec = QTextCodec::codecForName(encoding()); //"ISO-8859-2"; //?
	
	if (d->header.value("Content-Transfer-Encoding")=="quoted-printable")
	{
		send_body = messageBody(); // + "\n"; TODO: convert to charset
		send_body.replace("\n", "\r\n");
		send_body.replace("\n.", "\n=2E");
		send_body = Mime::encodeQP(send_body);
		
		//check for long lines
		uint long_line = longestLine(send_body);
		
		if (long_line > MAX_LINE_LEN)
		{
			send_body = breakLongLines(send_body, MAX_LINE_LEN);
		}
		
		//<crlf>.<crlf> is added by smtp, so this is not needed here
		//send_body += ".\r\n";
	}else{
		qDebug()<<"Unsupported Content-Transfer-Encoding: "<<header().value("Content-Transfer-Encoding");
	}
	//qDebug()<<send_body;
	return send_body;
};

/****************************************************************************/

void Mail::buildSendData(Format f)
{
	if (isParsed() || isSendDataValid() )
	{
		//do nothing if mail or mailpart was received
		return;
	}
	else
	{
		QString sd;
		
		//if (f != DataToSign)
		sd = getHeaderData();

		// if (isSinglePart())
		if (!getPartCount() || getPartCount()== 1)
		{
			//qDebug()<<isText();
			if(isText()) sd += "\r\n"+getBodySendDataForText()+"\r\n";
			else sd += "\r\n"+getBodySendData()+"\r\n";
		}
		else
		{
			sd += "\n\r\nThis is a multipart message in MIME format.\n\r\n";
			QString boundary = header().value("boundary");

			uint pc = getPartCount();
			bool init = false;
			
			for (uint i = 0; i< pc; i++)
			{
				Mail *part = getPart(i);

				sd += "--";
				sd += boundary;
				sd += "\r\n";

				init = true;
				part->buildSendData();
				sd += part->getSendData();

                                //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "++++++++++++++++++++++++++++++++++++++++++++++++++++++");
                                //qDebug (sd);
                                //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "++++++++++++++++++++++++++++++++++++++++++++++++++++++");
                                
				sd += "\r\n";
			}
			sd += "--";
			sd += boundary;
			sd += "--\r\n";

		}
		setSendData(sd);
		setSendDataValid( true );
	}
}

void Mail::setData(const QString& data)
{
	//qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "Mail::setData()");

	setSendData(data);
	parseData(data);
	setSendDataValid(true);
	d->is_parsed = true;
}

void Mail::parseData(const QString& data)
{
	QByteArray ba_data=data.toAscii();
	uint offset=parseHeader(ba_data);
	
	//qDebug()<<offset<<ba_data.size();
	
	QByteArray body_data = ba_data.mid(offset);
	//QMessageBox::information(0,"content",body_data);

	parseBody(body_data);
	
	d->is_parsed=true;
}

/****************************************************************************/

QString Mail::getSendData (Format f)
{
	if (!isSendDataValid ())
		buildSendData(f);

	return d->send_data;
}

QString Mail::getShowText()
{
	QString ret;
	QString cnt_type = d->header.value("Content-Type");
	
	if (isSinglePart())
	{
		if (isText())
		{
			if (cnt_type.contains("text/html") || cnt_type.contains("text/plain"))
			{
				ret +=messageBody();
			}else{
				if (d->header.value("Content-Disposition")=="attachment" || d->header.value("Content-Disposition")=="Attachment")
				{
					QString file_name = getFileName();
					ret += "\n<br><A href=\"part://"+getAddress()+"\">" 
						   + file_name + "</A><br>\n";
				}
				ret += messageBody();
			}
			
			if(d->header.value("Content-Transfer-Encoding")=="quoted-printable"){
				ret=Mime::decodeQP(ret);
			}
		}
		else if (cnt_type.contains("message/rfc822"))
		{
			Mail *mm = new Mail(this);
			addPart(mm);
			mm->setData( getSendData() );
			ret+= "\n<table border=\"1\"><tr><td>\n";
			ret+= "<small>"+tr("Embedded message") + "</small><BR>";
			//ret=tr("\n<br>---ORIGINAL MESSAGE---<br>\n");
			ret+=mm->getShowText(); //embedded
			ret+= "\n</td></tr></table>\n";
		}
		else if (cnt_type.contains("image"))
		{
			QString file_name = getFileName();
			QString name=saveInTemp();
			ret += "\n<br><img source=\""+ name + "\"><br><A href=\"file://"+name+"\">" + file_name + "</A><br>\n";
			
			//QString str(getRawData());
			//QImage img(str);
			//if (!img.isNull())
			//{
				//QString name = makeMimeSourceImg(img);
				//ret += "\n<br><img source=\""+ name + "\"><br>\n";
			//}
		} else {
			QString file_name = getFileName();
			ret += "\n<br><br><a href=\"file://"+saveInTemp()+"\" target=\"_blank\">" + file_name + "</a><br>\n";
		}
	}else if(cnt_type.contains("multipart/alternative")){
		//ret+="<meta http-equiv=\"content-type\" content=\"text/html; charset=" + encoding() +"\" /> \n";
		int index=-1;
		uint pc = getPartCount();
		for (uint i=0; i<pc; i++)
		{
			Mail* mp = getPart(i);
			if(mp->header("Content-Type").contains("text/html")){
				index=i;
				break;
			}else if(mp->header("Content-Type").contains("text/plain")){
				index=i;
			}
		}
		
		if(index > -1){
			Mail* mp = getPart(index);
			ret+= mp->getShowText();
		}
		
	}else {
		//ret+="<meta http-equiv=\"content-type\" content=\"text/html; charset=" + encoding() +"\" /> \n";
		uint pc = getPartCount();
		for (uint i=0; i<pc; i++)
		{
			Mail* mp = getPart(i);
			ret+= mp->getShowText();
		}
	}

	return ret;
}

/****************************************************************************/

void Mail::setSendData(const QString& data)
{
        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "Mail::setSendData()");
        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "~~~~~~~");
        //qDebug ("*** %s,%d : %s", __FILE__, __LINE__, (const char *)data);

	d->send_data = data;
}

void Mail::parseBody(QByteArray cstr_body)
{
	
	//QMessageBox::information(0,"content",body_data);
//qDebug()<<"single part?"<<isSinglePart();
	if (isSinglePart())
	{
		if (header().value("Content-Transfer-Encoding")=="quoted-printable")
		{
			setRawData(cstr_body);
		}
		else if (header().value("Content-Transfer-Encoding")=="base64")
		{
			//decode base63
			setRawData(QByteArray::fromBase64(cstr_body));
			if (isText())
			{
				cstr_body = qstrdup(d->raw_data.data());
			}
		}
		else
		{
			setRawData(cstr_body);
		}

		if (isText())
		{
			d->text_body = QString::fromAscii(cstr_body);
		}

		// BUG 762
		if (d->parent) setSendData(cstr_body);
	}
	else
	{
		QString body_data=QString::fromAscii(cstr_body);
		//qDebug("MULTIPART MAIL");
		QString bound = QString::fromLatin1("\n--") + header().value("boundary");
		uint bound_len = bound.length();

		//qDebug("BOUNDARY: '%s'", bound.latin1());
		int start = body_data.indexOf(bound);
		//while(body_data[start] != '\n' || body_data[start] != '\r'
		
		int end = 1;

		while(1){
			start += bound_len;
			if (body_data[start] == '\r')
				start++;
			if (body_data[start] == '\n')
				start++;

			end = body_data.indexOf(bound, start);
			if (end < 0)break;
			
			Mail* mp = new Mail(this);
			QString part_data = body_data.mid(start, end - start);
			//qDebug("PART DATA: ---\n'%s'---\n", (const char*) part_data);
			mp->setData(part_data);
			addPart(mp);
			start = end;
		};
	}
}

uint Mail::parseHeader(QByteArray data)
{
//qDebug()<<data;
	QTextStream ts(&data);
	QString line;
	QStringList lines;
	
	bool more = TRUE;
	while(more)
	{
		line = ts.readLine();
		//d->send_data += line;

		if (line.startsWith(" ") || line.startsWith("\t"))
		{
			lines.last() += line;
		}
		else
		{
			lines.append(line);
		}

		if (line == "")
		{
			more=false;
		}
		
	}

	for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
	{
		line = *it;
		//qDebug()<<"HEADER LINE: "<<line;
		parseHeaderLine(line);
	}
	
	if(header("boundary")!="") d->is_singlePart=false;
	
	return ts.pos();
}

void Mail::parseHeaderLine(QString line)
{
	/*
	int col_pos = line.indexOf(":");
	if (col_pos<1) col_pos=line.indexOf("=\"");
	if (col_pos < 1)
	{
		return;
	}
	
	QString field = line.left(col_pos);
	QString value = line.mid(col_pos+1,line.indexOf(";")-col_pos-1).trimmed();
	*/

	QStringList prop_list;
	prop_list<<"boundary"<<"charset"<<"name"<<"filename";
	for ( QStringList::Iterator it = prop_list.begin(); it != prop_list.end(); ++it )
	{
		QString prop = *it;
		int pos = line.indexOf(prop, FALSE);
		//qDebug("Searchnig for prop: %s, pos: %d", prop.latin1(), pos);
		if (pos>0)
		{
			int end;
			int start = pos+prop.length();
			
			if (line[start + 1] == '\"')
			{
				start+=2; 
				end = line.indexOf("\"", start);
				//qDebug("param starts with \", end: %d", end);
			}
			else
			{
				start++;
				end = line.indexOf(";", start);
				//qDebug("param find: ';' , end: %d", end);
				if (end < start)
				{
					end = line.indexOf(" ", start);
					//qDebug("param for: ' ' , end: %d", end);
				}
			}
			if (end < start)
			{
				end = line.length();
				//qDebug("param setting end at length, end: %d", end);
			}
			//qDebug("Searchnig for prop: %s, start: %d, end: %d", prop.latin1(), start, end);
//qDebug()<<"before: "<<line;
			QString prop_data = line.mid(start, end-start);
			QString left=line.left(pos-1);
			QString right=line.mid(end+1);
			line=left+right;
//qDebug()<<"after: "<<line;
			
			//qDebug("SETTING header parameter, '%s' '%s' '%s'", field.latin1(), prop.latin1(), prop_data.latin1());
//qDebug()<<prop<<prop_data;
			d->header.insert(prop,prop_data);
		}
	}

	int col_pos = line.indexOf(":");
	if (col_pos<1) col_pos=line.indexOf("=\"");
	if (col_pos<1) col_pos=line.indexOf("=");
	if (col_pos < 1)
	{
		return;
	}
	
	QString field = line.left(col_pos);
	int l;
	if(line.indexOf(";")>col_pos) l=line.indexOf(";")-col_pos-1;
	else l=-1;
	QString value = line.mid(col_pos+1,l).trimmed();

//qDebug()<<field<<value;
	d->header.insert(field,value);
}

uint Mail::getPartCount()
{
	return d->mail_parts.count();
}

Mail* Mail::getPart(uint index)
{
	return d->mail_parts.at(index);
}

/****************************************************************************/

uint Mail::addPart(Mail* mp)
{
	setSendDataValid(false);
	d->mail_parts.append(mp);
	d->is_singlePart=false;
	
	return d->mail_parts.count();
}

/****************************************************************************/

int Mail::findPart(Mail* mp)
{
	return d->mail_parts.indexOf(mp);
}

/****************************************************************************/

bool Mail::removePart(Mail* mp)
{
// Remove All parts
	if (!mp) {
		d->mail_parts.clear();
		setSendDataValid (false);
		d->is_singlePart=true;
		return true;
	} else {
		int pos = d->mail_parts.indexOf(mp);
		if (pos >= 0)
		{
			d->mail_parts.removeAt(pos);
			setSendDataValid(false);
			return true;
		}
		d->is_singlePart=d->mail_parts.count();
	}
	return false;
}

/****************************************************************************/

QByteArray Mail::getRawData()
{
	return d->raw_data;
}

void Mail::setRawData(const QByteArray& data)
{
	setSendDataValid(false);
	
	d->raw_data = data;
}

QString Mail::getAddress()
{
	QString out;
	
	if (d->parent)
	{
		int index = d->parent->findPart(this);
		if (index >= 0)
		{
			out = d->parent->getAddress()+"_"+QString("%1").arg(index);
		}
		else
		{
			qFatal("Mail::getAddress, part not found in parent");
		}
	}
	else
	{
		out = QString::fromLatin1("0");
	}
	return out;
}

Mail* Mail::getFromAddress(Mail* parent, const QString& address)
{
	QStringList al(address.split("_"));
	uint count = al.size();
	Mail* out = parent;
	//ommit first element it is parent
	for (uint i = 1; i<count; i++)
	{
		if (out == NULL)
		{
			qDebug()<<"Mail::getFromAddress, out is NULL, address: "<<address.toAscii();
		}
		out = out->getPart(al[i].toUInt());
	}
	if (out == NULL)
	{
		qDebug()<<"Mail::getFromAddress, out is NULL, address: "<<address.toAscii();
	}
	return out;
}

QString Mail::getHeaderData()
{
	d->header.insert("Date",timeStamp());
	QString data;
		
	QMapIterator<QString, QString> i(d->header);
	while (i.hasNext()) {
		i.next();
		if(i.key()=="Content-Type"){
			QString temp=QString("%1: %2").arg(i.key()).arg(i.value());
			if(d->header.value("charset")!="") temp.append("; charset=\""+d->header.value("charset")+"\"");
			if(!isSinglePart()) temp.append(";\n\tboundary=\""+d->header.value("boundary")+"\"");
			data.append(temp+"\r\n");
		}else if(i.key()=="charset" || i.key()=="boundary" || i.key()=="Bcc") continue;
		else data.append(QString("%1: %2\r\n").arg(i.key()).arg(i.value()));
	}
	data.append("\r\n");
//qDebug()<<data;
	return data;
}

QString Mail::contentType()
{
	return d->header.value("Content-Type");
}

bool Mail::isSendDataValid ()
{
	return d->is_send_data_valid;
}
	
void Mail::setSendDataValid(bool VAL)
{
	if (isParsed() && !VAL)
	{
		qWarning("Changing data in parsed Mail");
	}
	d->is_send_data_valid = VAL;
}

uint Mail::getMemSize()
{
	uint ret = d->send_data.size() + d->raw_data.size();
	uint pc = getPartCount();
	for (uint i=0; i<pc; i++)
	{
		ret+= getPart(i)->getMemSize();
	}
	
	return ret;
}
	
/*
void Mail::cleanTemp()
{
	if (!d->temp_file.isEmpty())
	{
		LHFileTools::removeTemp(d->temp_file);
	}
	
	uint pc = getPartCount();
	
	for (uint i = 0; i<pc; i++)
	{
		Mail* mp = getPart(i);
		mp->cleanTemp();
	}
}*/
	
QString Mail::saveInTemp()
{
	QString short_fname = getFileName();
	
	d->temp_file = new QTemporaryFile(this);
	d->temp_file->open();
	if(d->raw_data.isEmpty()) d->temp_file->write(d->send_data.toAscii());
	else d->temp_file->write(d->raw_data);
	QString fileName=d->temp_file->fileName();
	d->temp_file->close();
	return fileName;
}
	
QString Mail::makeMimeSourceImg(QImage img)
{
	d->mime_source = getAddress();
	//QMimeSourceFactory::defaultFactory()->setImage(d->mime_source, img);
	return d->mime_source;
}

QString Mail::timeStamp()
{
	/* Date: Mon, 08 May 2006 17:57:52 +0200 */
	/* Date: Sun, 28 May 2006 06:32:25 -0420 */
	QLocale english(QLocale::English,QLocale::UnitedKingdom);
	
	QDateTime dt = QDateTime::currentDateTime();
	QDate timecute = QDate::currentDate();
	QString day_en = english.dayName(timecute.dayOfWeek(),QLocale::ShortFormat);
	QString month_en = english.monthName(timecute.month(),QLocale::ShortFormat);
	QString last = dt.toString("yyyy hh:mm:ss");
	QString maildate = QString( "%1, %2 %3 %4 +0200" ).arg( day_en , QString::number(timecute.day()), month_en ,   last );
	return maildate;
}
