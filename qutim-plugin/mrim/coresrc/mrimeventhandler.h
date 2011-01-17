#ifndef MRIMEVENTHANDLER_H
#define MRIMEVENTHANDLER_H

#include "Status.h"
#include <qutim/plugininterface.h>

using namespace qutim_sdk_0_2;

class MRIMPluginSystem;

class MRIMEventHandlerClass : public EventHandler
{
public:
    static MRIMEventHandlerClass& Instance();
    MRIMEventHandlerClass();
    virtual ~MRIMEventHandlerClass();
    void processEvent(Event &event);
    void sendStatusChangedEvent(const QString& aAccount, const Status& aNewStatus);
    void sendConnectedEvent(const QString& aAccount, const Status& aConnectedStatus);
    void sendDisconnectedEvent(const QString& aAccount);

private:
    void HandleStatusChangeEvent(Event &event);

    MRIMPluginSystem *m_plugin_system;
    qint32 m_event_account_status_changed;
    qint32 m_event_account_connected;
    qint32 m_event_account_disconnected;
    qint32 m_event_account_status_change;
    qint32 m_event_account_status_text_change;
};

inline static MRIMEventHandlerClass& MRIMEventHandler()
{
    return MRIMEventHandlerClass::Instance();
}

#endif // MRIMEVENTHANDLER_H
