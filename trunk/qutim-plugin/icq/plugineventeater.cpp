#include "plugineventeater.h"
#include "icqlayer.h"

using namespace qutim_sdk_0_2;

PluginEventEater::PluginEventEater() : m_event_set_status(0xffff), m_event_restore_status(0xffff), m_event_set_xstatus(0xffff), m_event_restore_xstatus(0xffff)
{
}

PluginEventEater::~PluginEventEater()
{
}

void PluginEventEater::getEvent(const QList<void*> &event)
{
	if ( event.count() )
	{
		MessageToProtocolEventType type = *(MessageToProtocolEventType*)event.at(0);
		switch(type)
		{
		case SetStatus:
			setStatus(event);
			break;
		case RestoreStatus:
			restoreStatus(event);
			break;
		default:;
		}
	}
}

void PluginEventEater::processEvent(Event &ev)
{
	if(ev.id == 0xffff)
		return;
	if(!(ev.id == m_event_set_status || ev.id == m_event_set_xstatus
	   || ev.id == m_event_restore_status || ev.id == m_event_restore_xstatus) || ev.size() == 0)
		return;
	if(ev.id == m_event_set_status && ev.id < 3)
		return;
	if(ev.id == m_event_set_xstatus && ev.id < 4)
		return;
	QStringList &account_list = ev.at<QStringList>(0);
	if(account_list.empty())
	{
		foreach(icqAccount *account, m_icq_list)
		{
			if(ev.id == m_event_set_status)
				account->setStatusFromPlugin(ev.at<accountStatus>(1), ev.at<QString>(2));
			else if(ev.id == m_event_set_xstatus)
				account->setXstatusFromPlugin(ev.at<int>(1), ev.at<QString>(2), ev.at<QString>(3));
			else if(ev.id == m_event_restore_status)
				account->restoreStatusFromPlugin();
			else if(ev.id == m_event_restore_xstatus)
				account->restoreXstatusFromPlugin();
		}
	}
	else
	{
		foreach(const QString &account_uin, account_list)
		{
			icqAccount *account = m_icq_list.value(account_uin, 0);
			if(account)
			{
				if(ev.id == m_event_set_status)
					account->setStatusFromPlugin(ev.at<accountStatus>(1), ev.at<QString>(2));
				else if(ev.id == m_event_set_xstatus)
					account->setXstatusFromPlugin(ev.at<int>(1), ev.at<QString>(2), ev.at<QString>(3));
				else if(ev.id == m_event_restore_status)
					account->restoreStatusFromPlugin();
				else if(ev.id == m_event_restore_xstatus)
					account->restoreXstatusFromPlugin();
			}
		}
	}
}

void PluginEventEater::setAccountList(const QHash<QString, icqAccount*> &account_list )
{
	m_icq_list = account_list;
	if(m_event_set_status == 0xffff)
	{
		m_event_set_status = IcqPluginSystem::instance().registerEventHandler("ICQ/Account/Status/Set", this);
		m_event_restore_status = IcqPluginSystem::instance().registerEventHandler("ICQ/Account/Status/Restore", this);
		m_event_set_xstatus = IcqPluginSystem::instance().registerEventHandler("ICQ/Account/XStatus/Set", this);
		m_event_restore_xstatus = IcqPluginSystem::instance().registerEventHandler("ICQ/Account/XStatus/Restore", this);
	}
}

void PluginEventEater::setStatus(const QList<void *> &event)
{
	if ( event.count() > 3)
	{
		QStringList account_list = *(QStringList*)event.at(1);
		if ( !account_list.count() )
		{
			foreach (icqAccount *account, m_icq_list)
			{
				account->setStatusFromPlugin(*(accountStatus*)event.at(2), *(QString*)event.at(3));
				if ( event.count() > 6)
				{
					account->setXstatusFromPlugin(*(int*)event.at(4), *(QString*)event.at(5), *(QString*)event.at(6));
				}
			}
		}
		else{
			foreach( QString account, account_list)
			{
				icqAccount *paccount = m_icq_list.value(account);
				if ( paccount )
				{
					paccount->setStatusFromPlugin(*(accountStatus*)event.at(2), *(QString*)event.at(3));
					if ( event.count() > 6)
					{
						paccount->setXstatusFromPlugin(*(int*)event.at(4), *(QString*)event.at(5), *(QString*)event.at(6));
					}
				}
			}
		}
	}
}

void PluginEventEater::restoreStatus(const QList<void *> &event)
{
	if ( event.count() > 3)
	{
		QStringList account_list = *(QStringList*)event.at(1);
		if ( !account_list.count() )
		{
			foreach (icqAccount *account, m_icq_list)
			{
				account->restoreStatusFromPlugin();
				account->restoreXstatusFromPlugin();
			}
		}
		else{
			foreach( QString account, account_list)
			{
				icqAccount *paccount = m_icq_list.value(account);
				if ( paccount )
				{
					paccount->restoreStatusFromPlugin();
					paccount->restoreXstatusFromPlugin();
				}
			}
		}
	}
}
