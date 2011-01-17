#ifndef STATUSMANAGER_H
#define STATUSMANAGER_H

#include <QString>
#include <QObject>
#include <QList>
#include "Status.h"

class StatusManager : public QObject
{
    Q_OBJECT
public:
    static StatusManager& Instance();

    void ReadStatuses(const QString& aFileName);

    Status* GetStatus(const QString& aAccount, quint32 nStatus);
    Status* GetCustomStatus(const QString& aAccount, const QString& nStatusName);
    QString GetTooltip(quint32 nStatusID);
    QString GetTooltip(const QString& nStatusName);

protected:
    StatusManager();
    ~StatusManager();

private:
        
};

inline StatusManager& StatusMan()
{
    return StatusManager::Instance();
}

#endif // STATUSMANAGER_H
