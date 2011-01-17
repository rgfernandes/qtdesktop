/*
	HistoryManagerWindow

	Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "historymanagerwindow.h"
#include "chooseclientpage.h"
#include "clientconfigpage.h"
#include "importhistorypage.h"
#include "dumphistorypage.h"
#include "chooseordumppage.h"
#include <qutim/iconmanagerinterface.h>
#include <QLabel>
#include <QTextDocument>
#include <QComboBox>

namespace HistoryManager {

int inline compare_datetime_helper(const QDateTime &dt1, const QDateTime &dt2)
{
	QDateTime dtu1 = dt1.toUTC();
	QDateTime dtu2 = dt2.toUTC();
	QDate d1 = dtu1.date();
	QDate d2 = dtu2.date();
	if(d1 == d2)
		return dtu2.time().secsTo(dtu1.time());
	else
		return d2.daysTo(d1);
}

bool compare_message_helper(const Message &msg1, const Message &msg2)
{
	int cmp_d = compare_datetime_helper(msg1.time, msg2.time);
	if(!cmp_d)
	{
		int cmp_m = msg1.text.compare(msg2.text);
		if(!cmp_m)
			return msg1.in && !msg2.in;
		else
			return cmp_m < 0;
	}
	else
		return cmp_d < 0;
}

HistoryManagerWindow::HistoryManagerWindow(QWidget *parent) :
	QWizard(parent)
{
	m_protocol = 0;
	m_account = 0;
	m_contact = 0;
	m_current_client = 0;
	m_message_num = 0;
	m_qutim = new qutim();
	setPixmap(WatermarkPixmap, QPixmap(":/pictures/wizard.png"));
#ifndef Q_WS_MAC
	setWizardStyle(ModernStyle);
#endif
	setWindowTitle(tr("History manager"));
	setWindowIcon(qutim_sdk_0_2::Icon("history"));
	setPage(ChooseClient, new ChooseClientPage(this));
	setPage(ConfigClient, new ClientConfigPage(this));
	setPage(ImportHistory, new ImportHistoryPage(this));
	setPage(ChooseOrDump, new ChooseOrDumpPage(this));
	setPage(ExportHistory, new DumpHistoryPage(this));
//	qutim_sdk_0_2::SystemsCity::PluginSystem()->centerizeWidget(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	m_dump = tr("&Dump");
	m_is_dumping = false;
}

HistoryManagerWindow::~HistoryManagerWindow()
{
}

void HistoryManagerWindow::appendMessage(const Message &message)
{
	m_is_dumping = false;
	Q_ASSERT(m_contact);
	QDate date = message.time.date();
	qint64 month_id = date.year() * 100 + date.month();
	Month &month = m_contact->operator [](month_id);
	int position = qLowerBound(month.begin(), month.end(), message, compare_message_helper) - month.begin();
	if(month.size() != position
	   && month[position].time == message.time
	   && month[position].in   == message.in
	   && month[position].text == message.text)
		return;
	m_message_num++;
	month.insert(position, message);
}

void HistoryManagerWindow::setProtocol(const QString &protocol)
{
	m_is_dumping = false;
	m_protocol = &m_protocols.operator [](protocol);
}

void HistoryManagerWindow::setAccount(const QString &account)
{
	m_is_dumping = false;
	Q_ASSERT(m_protocol);
	m_account = &m_protocol->operator [](account);
}

void HistoryManagerWindow::setContact(const QString &contact)
{
	m_is_dumping = false;
	Q_ASSERT(m_account);
	m_contact = &m_account->operator [](contact);
}

void HistoryManagerWindow::setMaxValue(int max)
{
	if(m_is_dumping)
		emit saveMaxValueChanged(max);
	else
		emit maxValueChanged(max);
}

void HistoryManagerWindow::setValue(int value)
{
	if(m_is_dumping)
		emit saveValueChanged(value);
	else
		emit valueChanged(value);
}

ConfigWidget HistoryManagerWindow::createAccountWidget(const QString &protocol)
{
	QLabel *label = new QLabel;
	QString html = qutim_sdk_0_2::SystemsCity::IconManager()->getIconPath(protocol.toLower(), qutim_sdk_0_2::IconInfo::Protocol);
	if(html.isEmpty())
		html = protocol;
	else
	{
		QString tmp = Qt::escape(protocol);
		tmp += " <img src=\"";
		tmp += html;
		tmp += "\"/>";
		html = tmp;
	}
	label->setText(html);
	QComboBox *combo = new QComboBox;
	combo->setEditable(true);
	QList<qutim_sdk_0_2::TreeModelItem> accounts = qutim_sdk_0_2::SystemsCity::PluginSystem()->getItemChildren();
	foreach(const qutim_sdk_0_2::TreeModelItem &account, accounts)
	{
		if(account.m_protocol_name == protocol)
			combo->addItem(account.m_account_name, account.m_account_name);
	}
	return ConfigWidget(label, combo);
}

QString quoteByFormat(const QString &text, char format)
{
	if(format == 'j')
		return qutim::quote(text);
	else if(format == 'b')
		return QLatin1String(text.toUtf8().toHex());
	return text;
}

void HistoryManagerWindow::saveMessages(char format)
{
	if(format != 'b' && format != 'j')
		return;
	int total_count = 0;
	int num = 0;
	foreach(const Protocol &protocol, m_protocols)
		foreach(const Account &account, protocol)
			foreach(const Contact &contact, account)
				total_count += contact.size();
	emit saveMaxValueChanged(total_count);
	QString profile_path = qutim_sdk_0_2::SystemsCity::PluginSystem()->getProfilePath();
	QDir dir(profile_path + QDir::separator() + "history");
	if(!dir.exists())
		QDir(profile_path).mkpath(profile_path);

	QHash<QString, Protocol>::const_iterator protocol = m_protocols.constBegin();
	for(; protocol != m_protocols.constEnd(); protocol++)
	{
		QHash<QString,Account>::const_iterator account = protocol.value().constBegin();
		for(; account != protocol.value().constEnd(); account++)
		{
			QString account_path = protocol.key();
			account_path += ".";
			account_path += quoteByFormat(account.key(), format);
			if(!dir.exists(account_path))
				dir.mkdir(account_path);
			QDir account_dir = dir.filePath(account_path);
			QHash<QString,Contact>::const_iterator contact = account.value().constBegin();
			for(; contact != account.value().constEnd(); contact++)
			{
				QMap<qint64,Month>::const_iterator month = contact.value().constBegin();
				for(; month != contact.value().constEnd(); month++)
				{
					emit saveValueChanged(++num);
					QString filename = quoteByFormat(contact.key(), format);
					filename += ".";
					filename += QString::number(month.key());
					if(format == 'j')
						filename += ".json";
					else
						filename += ".log";
					QFile file(account_dir.filePath(filename));
					if(format == 'j')
					{
						if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
							continue;
						file.write("[\n");
						bool first = true;
						foreach(const Message &message, month.value())
						{
							if(first)
								first = false;
							else
								file.write(",\n");
							file.write(" {\n  \"datetime\": \"");
							file.write(message.time.toString(Qt::ISODate).toLatin1());
							file.write("\",\n  \"type\": ");
							file.write(QString::number(message.type).toLatin1());
							file.write(",\n  \"in\": ");
							file.write(message.in ? "true" : "false");
							file.write(",\n  \"text\": ");
							file.write(K8JSON::quote(message.text).toUtf8());
							file.write("\n }");
						}
						file.write("\n]");
					}
					else if(format == 'b')
					{
						if(!file.open(QIODevice::WriteOnly))
							continue;
						QDataStream out(&file);
						foreach(const Message &msg, month.value())
							out << msg.time << msg.type << msg.in << msg.text;
					}
				}
			}
		}
	}
}

void HistoryManagerWindow::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		setWindowTitle(tr("History manager"));
		break;
	default:
		break;
	}
}

}
