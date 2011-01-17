#include "useragent.h"
#include <QRegExp>
#include <QIcon>
#include <QStringList>
#include <qutim/iconmanagerinterface.h>

using namespace qutim_sdk_0_2;

UserAgent::UserAgent( const QString &aClientID, const QString &aVersionStr, const QString &aBuildVer, quint8 nProtoMajorVer, quint8 nProtoMinorVer )
: m_clientID(aClientID), m_versionStr(aVersionStr), m_buildVer(aBuildVer), m_protoMajorVer(nProtoMajorVer), m_protoMinorVer(nProtoMinorVer)
{
}

UserAgent::UserAgent()
: m_clientID(""), m_versionStr(""), m_buildVer(""), m_protoMajorVer(0), m_protoMinorVer(0)
{
}

UserAgent::~UserAgent()
{
}

UserAgent* UserAgent::Parse( const QString &aUserAgentStr)
{
    if (aUserAgentStr.isEmpty()) return new UserAgent();
    QRegExp rx("((\\w+)=\\\"([\\w \\t\\.]+)\\\"*)+");
    QString clientID, versionStr, buildVer;
    quint8 protoVerMinor = 0, protoVerMajor = 0;

    quint32 offset = 0;
    QString currParam;
    while ((offset = rx.indexIn(aUserAgentStr, offset)) != -1)
    {
//        qDebug()<<rx.capturedTexts();
        currParam = rx.cap(2);

        if (currParam == "client")
            clientID = rx.cap(3);
        else if (currParam == "version")
            versionStr = rx.cap(3);
        else if (currParam == "build")
            buildVer = rx.cap(3);
        else if (currParam == "protocol")
        {
            QRegExp verRx("(\\d+)\\.(\\d+)");

            if (verRx.exactMatch(rx.cap(3)))
            {
                protoVerMajor = verRx.cap(1).toUInt();
                protoVerMinor = verRx.cap(2).toUInt();
            }
        }
        offset += rx.matchedLength();
    }

    return new UserAgent(clientID,versionStr,buildVer,protoVerMajor,protoVerMinor);
}

QString UserAgent::Stringify() const
{
    QString protoVerStr = QString("%1.%2").arg(m_protoMajorVer).arg(m_protoMinorVer);
    QString userAgentFull = QString("client=\"%1\" version=\"%2\" build=\"svn%3\"").arg(m_clientID).arg(m_versionStr).arg(m_buildVer);
    return userAgentFull;   
}

QString UserAgent::HumanReadable() const
{
    QString userAgentFmt("%1 %2 %3");
    return userAgentFmt.arg(m_clientID).arg(m_versionStr).arg(m_buildVer);
}

bool UserAgent::IsEmpty() const
{
    return (m_clientID.isEmpty());
}

QIcon UserAgent::GetIcon() const
{
    QString icon_name = m_clientID.toLower();

    if(icon_name.isEmpty())
            return QIcon();
    else if(icon_name.startsWith("qutim"))
            icon_name="qutim";
    else if(icon_name.startsWith("qip infium"))
            icon_name="qipinf";
    else if(icon_name.startsWith("magent"))
            icon_name="magent";
    else if(icon_name.startsWith("mchat"))
            icon_name="mchat";
    else if(icon_name.startsWith("imadering"))
            icon_name="imadering";
    else if(icon_name.startsWith("mraqt"))
            icon_name="mraqt";
    else if(icon_name.startsWith("miranda"))
            icon_name="miranda";
    else if(icon_name.startsWith("psi"))
            icon_name="psi";
    else if(icon_name.startsWith("pidgin"))
            icon_name="pidgin";
    else if(icon_name.startsWith("mdc"))
            icon_name="mdc";

    QIcon icon = Icon( icon_name, IconInfo::Client );

    if( icon.isNull() )
            icon = Icon( "unknown", IconInfo::Client );
    return icon;
}

void UserAgent::Set(const UserAgent& aNewAgent)
{
    m_clientID = aNewAgent.m_clientID;
    m_versionStr = aNewAgent.m_versionStr;
    m_buildVer = aNewAgent.m_buildVer;
    m_protoMajorVer = aNewAgent.m_protoMajorVer;
    m_protoMinorVer = aNewAgent.m_protoMinorVer;
    emit Changed();
}
