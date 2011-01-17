#include "avatarfetcher.h"
#include <qdebug.h>
#include <QSettings>
#include "MRIMPluginSystem.h"

AvatarFetcher* AvatarFetcher::m_instance = 0;

AvatarFetcher::AvatarFetcher() : QObject(0)
{
    m_headerHttp = new QHttp("obraz.foto.mail.ru",80);
    m_avaHttp = new QHttp("obraz.foto.mail.ru",80);

    connect(m_headerHttp,SIGNAL(responseHeaderReceived(QHttpResponseHeader)),this,SLOT(HandleAvatarRequestHeader(QHttpResponseHeader)));
    connect(m_avaHttp,SIGNAL(requestFinished(int, bool)),this,SLOT(HandleAvatarFetched(int,bool)));
}

AvatarFetcher::~AvatarFetcher()
{
    disconnect(m_headerHttp,SIGNAL(responseHeaderReceived(QHttpResponseHeader)),this,SLOT(HandleAvatarRequestHeader(QHttpResponseHeader)));
    disconnect(m_avaHttp,SIGNAL(requestFinished(int, bool)),this,SLOT(HandleAvatarFetched(int,bool)));
    delete m_headerHttp;
    delete m_avaHttp;
}

void AvatarFetcher::FetchSmallAvatar(const QString& aEmail)
{
    QRegExp rx("(.+)@(.+).ru");
    rx.indexIn(aEmail,0);
    if (rx.numCaptures() < 2 )
    {
#ifdef DEBUG_LEVEL_DEV
//        qDebug()<<"Regexp failed";
#endif
        return;
    }
    QStringList list = rx.capturedTexts();
    if (list[1] == "" || list[2] == "") return;
    QString req = QString("http://obraz.foto.mail.ru/%1/%2/_mrimavatarsmall").arg(list[2]).arg(list[1]);
#ifdef DEBUG_LEVEL_DEV
//    qDebug()<<"Fetching small avatar header from URL: "<<req;
#endif
    qint32 reqId = m_headerHttp->head(req);
    m_smallAvatarReqIds.insert(aEmail,reqId);
}

void AvatarFetcher::FetchBigAvatar(const QString& aEmail)
{
    QRegExp rx("(.+)@(.+).ru");
    rx.indexIn(aEmail,0);
    if (rx.numCaptures() < 2 )
    {
#ifdef DEBUG_LEVEL_DEV
        qDebug()<<"Regexp failed";
#endif
        return;
    }
    QStringList list = rx.capturedTexts();
    if (list[1] == "" || list[2] == "") return;
    QString req = QString("http://obraz.foto.mail.ru/%1/%2/_mrimavatar").arg(list[2]).arg(list[1]);
#ifdef DEBUG_LEVEL_DEV
    //qDebug()<<"Fetching big avatar header from URL: "<<req;
#endif
    qint32 reqId = m_headerHttp->head(req);
    m_bigAvatarReqIds.insert(aEmail,reqId);
}

void AvatarFetcher::HandleAvatarRequestHeader(const QHttpResponseHeader& resp)
{
#ifdef DEBUG_LEVEL_DEV
//    qDebug()<<"HTTP Request finished";
#endif
    if (resp.statusCode() != 404)
    {
        bool smallAvatar = m_smallAvatarReqIds.values().contains(m_headerHttp->currentId());
        QString email = (smallAvatar) ? m_smallAvatarReqIds.key(m_headerHttp->currentId()) :
                                        m_bigAvatarReqIds.key(m_headerHttp->currentId());
        if (smallAvatar)
            m_smallAvatarReqIds.remove(email);
        else
            m_bigAvatarReqIds.remove(email);

        QString reqType = (smallAvatar) ? "small avatar req" : "big avatar req";
#ifdef DEBUG_LEVEL_DEV
//        qDebug()<<"Request type is "+reqType;
#endif
        bool willFetch = true;
        QString postfix;
        if (smallAvatar) postfix = "small";

        if (resp.hasKey("Date"))
        {
            QSettings avatarsCacheSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+MRIMPluginSystem::ImplPointer()->Profile()+"/mrimicons", "avatars_cache");
            QString currentDate = resp.value("Last-Modified");
            QString lastFetchDate = avatarsCacheSettings.value(email+"/last"+postfix+"AvatarFetchDate").toString();
#ifdef DEBUG_LEVEL_DEV
//            qDebug()<<"Avatar last fetch date is "<<lastFetchDate;
//            qDebug()<<"Avatar current date is "<<currentDate;
#endif
            if (currentDate == lastFetchDate)
            {
                if (smallAvatar)
                    willFetch = ( !QFile::exists(SmallAvatarPath(email)) );
                else
                    willFetch = ( !QFile::exists(BigAvatarPath(email)) );
            }
            else
                avatarsCacheSettings.setValue(email+"/last"+postfix+"AvatarFetchDate",currentDate);
        }

        if (willFetch)
        {
#ifdef DEBUG_LEVEL_DEV
//            qDebug()<<"Will fetch avatar now!";
#endif
            QRegExp rx("(.+)@(.+).ru");
            rx.indexIn(email,0);
            if (rx.numCaptures() < 2)
            {
#ifdef DEBUG_LEVEL_DEV
//                qDebug()<<"Regexp failed";
#endif
                return;
            }
            QStringList list = rx.capturedTexts();
            if (list[1] == "" || list[2] == "") return;
            QString path = QString("http://obraz.foto.mail.ru/%1/%2/_mrimavatar%3").arg(list[2]).arg(list[1]).arg(postfix);
#ifdef DEBUG_LEVEL_DEV
//            qDebug()<<"Fetching avatar from URL: "<<path;
#endif
            if (smallAvatar)
            {
                qint32 reqId = m_avaHttp->get(path);
                m_smallAvatarReqIds.insert(email,reqId);
            }
            else
            {
                qint32 reqId = m_avaHttp->get(path);
                m_bigAvatarReqIds.insert(email,reqId);
            }
        }
        else
        {
#ifdef DEBUG_LEVEL_DEV
//            qDebug()<<"Avatar will not be fetched!";
#endif
        }
    }
}

void AvatarFetcher::HandleAvatarFetched(int aReqId, bool aError)
{
    bool smallAvatar = m_smallAvatarReqIds.values().contains(aReqId);
    QString email = (smallAvatar) ? m_smallAvatarReqIds.key(aReqId) :
                                    m_bigAvatarReqIds.key(aReqId);
    if (smallAvatar)
        m_smallAvatarReqIds.remove(email);
    else
        m_bigAvatarReqIds.remove(email);

    if (aError || email.isEmpty()) return;

    if (smallAvatar)
    {
        QString fileName = SmallAvatarPath(email);
#ifdef DEBUG_LEVEL_DEV
//        qDebug()<<"Fetching small avatar to file: "<<fileName;
#endif
        QFile smallAvatar(fileName);
        smallAvatar.open(QIODevice::ReadWrite);
        qint64 written = smallAvatar.write(m_avaHttp->readAll());
        smallAvatar.waitForBytesWritten(5000);
        if (written < 50) smallAvatar.remove(); //50 is a hack for support@corp.mail.ru
        smallAvatar.close();
        emit SmallAvatarFetched(email);
    }
    else
    {
        if (aError || email.isEmpty()) return;

        QString fileName = BigAvatarPath(email);
#ifdef DEBUG_LEVEL_DEV
//        qDebug()<<"Fetching big avatar to file: "<<fileName;
#endif
        QFile bigAvatar(fileName);
        bigAvatar.open(QIODevice::ReadWrite);
        qint64 written = bigAvatar.write(m_avaHttp->readAll());
        bigAvatar.waitForBytesWritten(5000);
        if (written < 50) bigAvatar.remove(); //50 is a hack for support@corp.mail.ru
        bigAvatar.close();
        emit BigAvatarFetched(email);
    }
}

QString AvatarFetcher::SmallAvatarPath(const QString& aEmail)
{
    QSettings avatarsCacheSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+MRIMPluginSystem::ImplPointer()->Profile()+"/mrimicons", "avatars_cache");
    QString avatarsDir = avatarsCacheSettings.fileName().section('/',0,-2)+"/";
    QString fileName = avatarsDir  + aEmail + "_small.jpg";
    QDir dir(avatarsDir);
    if (!dir.exists())
    {
        dir.mkpath(avatarsDir);
    }

    return fileName;
}

QString AvatarFetcher::BigAvatarPath(const QString& aEmail)
{
    QSettings avatarsCacheSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+MRIMPluginSystem::ImplPointer()->Profile()+"/mrimicons", "avatars_cache");
    QString avatarsDir = avatarsCacheSettings.fileName().section('/',0,-2)+"/";
    QString fileName = avatarsDir  + aEmail + "_big.jpg";
    QDir dir(avatarsDir);
    if (!dir.exists())
    {
        dir.mkpath(avatarsDir);
    }
    return fileName;
}

