#ifndef USERAGENT_H
#define USERAGENT_H

#include <QObject>
#include <QString>
class QIcon;

class UserAgent : public QObject
{
    Q_OBJECT
public:
    UserAgent();
    UserAgent( const QString &aClientID, const QString &aVersionStr, const QString &aBuildVer, quint8 nProtoMajorVer, quint8 nProtoMinorVer );
    ~UserAgent();

    static UserAgent* Parse(const QString &aUserAgentStr);
    QString Stringify() const;
    QString HumanReadable() const;
    bool IsEmpty() const;
    QIcon GetIcon() const;
    void Set(const UserAgent& aNewAgent);
signals:
    void Changed();
private:
    //Client ID
    QString m_clientID;
    //Client version
    QString m_versionStr;
    QString m_buildVer;
    //Protocol version
    quint8 m_protoMajorVer;
    quint8 m_protoMinorVer;
};

#endif // USERAGENT_H
