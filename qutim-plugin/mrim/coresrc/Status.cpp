#include "Status.h"
#include <qutim/iconmanagerinterface.h>
using namespace qutim_sdk_0_2;

//StatusData class implementation
StatusData::StatusData(quint32 aStatus, const QString &aTitle, const QString &aDescription, const QString &aCustomID)
: m_status(aStatus), m_title(aTitle), m_descr(aDescription)
{    
    m_customStatusID = aCustomID;
    m_customStatusID.remove("status_");
}

StatusData::StatusData( quint32 aStatus )
: m_status(aStatus)
{    
}

bool operator==( const StatusData& aStatusData, const StatusData& aOtherStatusData )
{
    bool numOk = (aStatusData == aOtherStatusData.m_status);
    bool titleOk = (aStatusData.m_title == aOtherStatusData.m_title);
    bool descrOk = (aStatusData.m_descr == aOtherStatusData.m_descr);
    bool customIDOk = (aStatusData.m_customStatusID == aOtherStatusData.m_customStatusID);

    return (numOk && titleOk && descrOk && customIDOk);   
}

bool operator!=( const StatusData& aStatusData, const StatusData& aOtherStatusData )
{
    return !operator==(aStatusData,aOtherStatusData);
}

bool operator==( const StatusData& aStatusData, quint32 aOtherStatus )
{
    return (aStatusData.m_status == aOtherStatus);
}

bool operator!=( const StatusData& aStatusData, quint32 aOtherStatus )
{
    return (aStatusData.m_status != aOtherStatus);
}

//Status class implementation
Status::Status(quint32 aStatus, const QString &aTitle, const QString &aDescription, const QString &aCustomID) 
: m_statusData(aStatus, aTitle, aDescription, aCustomID)
{
}

Status::Status( const StatusData &aStatusData ) : m_statusData(aStatusData)
{
}

Status::~Status()
{
}

QIcon Status::GetIcon() const
{
    return GetIcon(m_statusData.m_status, m_statusData.m_customStatusID);
}

QIcon Status::GetIcon( quint32 nStatus, const QString &aCustomStatusID )
{
    return GetIcon(Stringify(nStatus,aCustomStatusID));
}

QIcon Status::GetIcon( const QString &aStatusName )
{
    return Icon(aStatusName,IconInfo::Status,"mrim");
}

QString Status::GetIconPath() const
{
    return GetIconPath(m_statusData.m_status, m_statusData.m_customStatusID);
}

QString Status::GetIconPath(quint32 nStatus, const QString &aCustomStatusID)
{
    return SystemsCity::IconManager()->getIconPath(Stringify(nStatus,aCustomStatusID),IconInfo::Status,"mrim");
}

QString Status::Stringify() const
{
    return Stringify(m_statusData.m_status,m_statusData.m_customStatusID);
}

QString Status::Stringify( quint32 nStatus, const QString &aCustomStatusID)
{
    quint32 nStrippedStatus = nStatus; //TODO: to cut the invisible flag
    QString cuttedCustomId = aCustomStatusID.toLower();
    cuttedCustomId.remove("status_");

    switch (nStrippedStatus)
    {
    case STATUS_ONLINE:
        return "STATUS_ONLINE";
    case STATUS_AWAY:
        return "STATUS_AWAY";
    case STATUS_OFFLINE:
        return "STATUS_OFFLINE";
    case STATUS_FLAG_INVISIBLE:
        return "STATUS_INVISIBLE";
    case STATUS_USER_DEFINED:
        return ("status_" + cuttedCustomId);
    default:
        return "STATUS_UNDETERMINATED";
    }

    return QString();
}

quint32 Status::FromString(const QString& aString)
{
    QString lowerStatus = aString.toLower();

    if  (aString == "status_online")
        return STATUS_ONLINE;
    else if (aString == "status_away")
        return STATUS_AWAY;
    else if (aString == "status_invisible")
        return STATUS_FLAG_INVISIBLE;
    else if (aString == "status_offline")
        return STATUS_OFFLINE;
    else return STATUS_USER_DEFINED;

    return STATUS_UNDETERMINATED;
}

StatusData Status::GetData() const
{
    return m_statusData;
}

quint32 Status::Get() const
{
    return m_statusData.m_status;
}

void Status::Set( quint32 nNewStatus, const QString &aCustomID )
{
    m_statusData.m_status = nNewStatus;
    
    if (nNewStatus == STATUS_USER_DEFINED)
    { 
        m_statusData.m_customStatusID = aCustomID;
    }
    emit Changed();
}

QString Status::GetTitle() const
{
    return m_statusData.m_title;
}

void Status::SetTitle( const QString& aTitle )
{
    m_statusData.m_title = aTitle;
    emit Changed();
}

QString Status::GetDescription() const
{
    return m_statusData.m_descr;
}

void Status::SetDescription( const QString& aDescription )
{
    m_statusData.m_descr = aDescription;
    emit Changed();
}

Status& Status::operator=( quint32 aNewStatus )
{
    Set(aNewStatus);
    return *this;
}

Status& Status::operator=( const StatusData &aNewStatusData )
{
    m_statusData = aNewStatusData;
    return *this;
}

void Status::Clone( const Status &aNewStatus, bool aNotifyChange )
{
    m_statusData = aNewStatus.m_statusData;

    if (aNotifyChange)
    {
        emit Changed();
    }
}

void Status::Clear()
{
    m_statusData = StatusData(STATUS_UNDETERMINATED,"","","");
}

void Status::SetCustomID( const QString &aCustomID )
{
    m_statusData.m_customStatusID = aCustomID;
    m_statusData.m_customStatusID.remove("status_");
}

quint32 Status::GetMass( quint32 aNumStatus, const QString &aCustomID )
{
    int mass;
    switch (aNumStatus)
    {
    case STATUS_ONLINE:
        mass = 0;
        break;
    case STATUS_AWAY:
        mass = 2;
        break;
    case STATUS_FLAG_INVISIBLE:
        mass = 11;
        break;
    case STATUS_USER_DEFINED:
        {
            mass = 20;
            bool ok;
            quint32 custId = aCustomID.toUInt(&ok);
            
            if (ok)
            {
                mass = 30;
                mass += custId;
            }
            else if (aCustomID == "chat")
            {
                mass += 5;
            }
        }
        break;
    default:
        mass = 1000;
        break;
    }
    return mass;
}

quint32 Status::GetMass()
{
    return GetMass(m_statusData.m_status,m_statusData.m_customStatusID);
}

bool operator==( const Status& aStatus, quint32 aOtherStatus )
{
    return (aStatus.m_statusData == aOtherStatus);
}

bool operator!=( const Status& aStatus, quint32 aOtherStatus )
{
    return (aStatus.m_statusData != aOtherStatus);
}

bool operator==( const Status& aStatus, const Status& aOtherStatus )
{
    return (aStatus.m_statusData == aOtherStatus.m_statusData);
}

bool operator!=( const Status& aStatus, const Status& aOtherStatus )
{
    return (aStatus.m_statusData != aOtherStatus.m_statusData);
}
