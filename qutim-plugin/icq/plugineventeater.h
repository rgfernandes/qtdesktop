#ifndef PLUGINEVENTEATER_H_
#define PLUGINEVENTEATER_H_

#include "icqaccount.h"

using namespace qutim_sdk_0_2;

class PluginEventEater : public EventHandler
{	
public:
	void getEvent(const QList<void*> &event);
	void processEvent(Event &ev);
	void setAccountList(const QHash<QString, icqAccount*> &account_list );
private:
	PluginEventEater();
	virtual ~PluginEventEater();
	QHash<QString, icqAccount *> m_icq_list;
	void setStatus(const QList<void *> &event);
	void restoreStatus(const QList<void *> &event);
	quint16 m_event_set_status;
	quint16 m_event_restore_status;
	quint16 m_event_set_xstatus;
	quint16 m_event_restore_xstatus;
	friend class IcqLayer;
};

#endif /*PLUGINEVENTEATER_H_*/
