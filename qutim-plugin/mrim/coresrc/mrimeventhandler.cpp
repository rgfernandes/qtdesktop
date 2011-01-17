#include "mrimeventhandler.h"
#include "MRIMPluginSystem.h"
#include "proto.h"
#include <QStringList>

using namespace qutim_sdk_0_2;

MRIMEventHandlerClass::MRIMEventHandlerClass()
{
    m_plugin_system = PluginSystemImpl();
    m_event_account_status_changed = m_plugin_system->PluginSystem()->registerEventHandler("MRIM/Account/Status/Changed");
    m_event_account_connected = m_plugin_system->PluginSystem()->registerEventHandler("MRIM/Account/Connected");
    m_event_account_disconnected = m_plugin_system->PluginSystem()->registerEventHandler("MRIM/Account/Disconnected");
    m_event_account_status_change = m_plugin_system->PluginSystem()->registerEventHandler("MRIM/Account/Status/Change", this);
    m_event_account_status_text_change = m_plugin_system->PluginSystem()->registerEventHandler("MRIM/Account/Status/ChangeText", this);
}

MRIMEventHandlerClass& MRIMEventHandlerClass::Instance()
{
    return *MRIMPluginSystem::ImplPointer()->GetEventHandler();
}

MRIMEventHandlerClass::~MRIMEventHandlerClass()
{
    m_plugin_system = NULL;
}

void MRIMEventHandlerClass::processEvent(Event &event)
{
    if (event.id == m_event_account_status_change || event.id == m_event_account_status_text_change)
    {
        HandleStatusChangeEvent(event);
    }
}

void MRIMEventHandlerClass::sendStatusChangedEvent(const QString& aAccount, const Status& aNewStatus)
{
    StatusData data = aNewStatus.GetData();
    Event event(m_event_account_status_changed,5,&aAccount,&data.m_status,&data.m_customStatusID,&data.m_title,&data.m_descr);
    m_plugin_system->PluginSystem()->sendEvent(event);
}

void MRIMEventHandlerClass::sendConnectedEvent(const QString& aAccount, const Status& aConnectedStatus)
{
    Event event(m_event_account_connected,1,&aAccount);
    m_plugin_system->PluginSystem()->sendEvent(event);
    sendStatusChangedEvent(aAccount,aConnectedStatus);
}

void MRIMEventHandlerClass::sendDisconnectedEvent(const QString& aAccount)
{
    Event event(m_event_account_disconnected,1,&aAccount);
    m_plugin_system->PluginSystem()->sendEvent(event);
}

void MRIMEventHandlerClass::HandleStatusChangeEvent(Event &event)
{
    if (event.args.size() < 1)
            return;

    QStringList *accountsList = (QStringList*)event.args.at(0);

    if (accountsList == 0)
    {
        return;
    }

    Status newStatus;

    MRIMClient *account = 0;
    QString *statusCustomId = 0;
    QString *statusTitle = 0;
    QString *statusDesc = 0;
    qint32 *statusNum = 0;

    if (event.id == m_event_account_status_change)
    {
        statusNum = (qint32*)event.args.at(1);
        statusCustomId = (QString*)event.args.at(2);
        statusTitle = (QString*)event.args.at(3);
        statusDesc = (QString*)event.args.at(4);
   }
    else if (event.id == m_event_account_status_text_change)
    {
        statusTitle = (QString*)event.args.at(1);
        statusDesc = (QString*)event.args.at(2);
    }

    foreach (QString acc, *accountsList)
    {
        account = PluginSystemImpl()->FindClientInstance(acc);

        if (account && account->Protocol()->IsOnline())
        {
            newStatus = account->Protocol()->CurrentStatus().GetData(); //get current status, then change it

            if (statusNum)
            {
                newStatus.Set(*statusNum);

                if (newStatus.Get() == STATUS_USER_DEFINED)
                {
                    newStatus.SetCustomID(*statusCustomId);
                }
            }

            if (statusTitle)
            {
                newStatus.SetTitle(*statusTitle);
            }

            if (statusDesc)
            {
                newStatus.SetDescription(*statusDesc);
            }
            account->ChangeStatus(newStatus);
        }
    }
}

