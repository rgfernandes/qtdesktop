#ifndef AVATARFETCHER_H
#define AVATARFETCHER_H

#include <QObject>
#include <QHttp>
#include <QHash>
#include <QFile>
#include <QStringList>

class AvatarFetcher : public QObject
{
    Q_OBJECT
public:
    static AvatarFetcher* Instance()
    {
        if (!m_instance)
        {
            m_instance = new AvatarFetcher;
        }
        return m_instance;
    }

    AvatarFetcher();
    ~AvatarFetcher();

    void FetchSmallAvatar(const QString& aEmail);
    void FetchBigAvatar(const QString& aEmail);

    static QString SmallAvatarPath(const QString& aEmail);
    static QString BigAvatarPath(const QString& aEmail);
signals:
    void SmallAvatarFetched(QString aEmail);
    void BigAvatarFetched(QString aEmail);
protected slots:
    void HandleAvatarRequestHeader(const QHttpResponseHeader& resp);
    void HandleAvatarFetched(int aReqId, bool aError);
protected:
    static AvatarFetcher* m_instance;

    QHttp* m_headerHttp;
    QHttp* m_avaHttp;

    QHash<QString,qint32> m_smallAvatarReqIds;
    QHash<QString,qint32> m_bigAvatarReqIds;
};

#endif // AVATARFETCHER_H
