#include "statusmanager.h"
#include <QSettings>
#include "MRIMPluginSystem.h"

StatusManager::StatusManager()
{

}

StatusManager::~StatusManager()
{

}

QString StatusManager::GetTooltip( const QString& nStatusName )
{
    QStringList splitted(nStatusName.split('_'));
    if (splitted.count() < 2) return "?";
    QString lowerId = splitted[1].toLower();

    if (lowerId == "offline")
        return tr("Offline");
    else if (lowerId == "dnd")
        return tr("Do Not Disturb");
    else if (lowerId == "chat")
        return tr("Free For Chat");
    else if (lowerId == "online")
        lowerId = "1";
    else if (lowerId == "away")
        lowerId = "2";
    else if (lowerId == "invisible")
        lowerId = "3";

    return GetTooltip(lowerId.toUInt());
}

QString StatusManager::GetTooltip(quint32 nStatusID )
{
    switch (nStatusID)
    {    
    case 1:
        return tr("Online");
    case 2:
        return tr("Away");
    case 3:
        return tr("Invisible");
    case 4:
        return tr("Sick");
    case 5:
        return tr("At home");
    case 6:
        return tr("Lunch");
    case 7:
        return tr("Where am I?");
    case 8:
        return tr("WC");
    case 9:
        return tr("Cooking");
    case 10:
        return tr("Walking");
    case 11:
        return tr("I'm an alien!");
    case 12:
        return tr("I'm a shrimp!");
    case 13:
        return tr("I'm lost :(");
    case 14:
        return tr("Crazy %)");
    case 15:
        return tr("Duck");
    case 16:
        return tr("Playing");
    case 17:
        return tr("Smoke");
    case 18:
        return tr("At work");
    case 19:
        return tr("On the meeting");
    case 20:
        return tr("Beer");
    case 21:
        return tr("Coffee");
    case 22:
        return tr("Shovel");
    case 23:
        return tr("Sleeping");
    case 24:
        return tr("On the phone");
    case 26:
        return tr("In the university");
    case 27:
        return tr("School");
    case 28:
        return tr("You have the wrong number!");
    case 29:
        return tr("LOL");
    case 30:
        return tr("Tongue");
    case 32:
        return tr("Smiley");
    case 33:
        return tr("Hippy");
    case 34:
        return tr("Depression");
    case 35:
        return tr("Crying");
    case 36:
        return tr("Surprised");
    case 37:
        return tr("Angry");
    case 38:
        return tr("Evil");
    case 39:
        return tr("Ass");
    case 40:
        return tr("Heart");
    case 41:
        return tr("Crescent");
    case 42:
        return tr("Coool!");
    case 43:
        return tr("Horns");
    case 44:
        return tr("Figa");
    case 45:
        return tr("F*ck you!");
    case 46:
        return tr("Skull");
    case 47:
        return tr("Rocket");
    case 48:
        return tr("Ktulhu");
    case 49:
        return tr("Goat");
    case 50:
        return tr("Must die!!");
    case 51:
        return tr("Squirrel");
    case 52:
        return tr("Party!");
    case 53:
        return tr("Music");
    default:
        return tr("?");
    }
}

StatusManager& StatusManager::Instance()
{
    static StatusManager myInstance;
    return myInstance;
}

Status *StatusManager::GetStatus(const QString& aAccount, quint32 nStatus)
{
    return GetCustomStatus(aAccount,Status::Stringify(nStatus));
}

Status *StatusManager::GetCustomStatus(const QString& aAccount, const QString& nStatusName)
{
    QSettings acc_settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+PluginSystemImpl()->Profile()+"/mrim."+aAccount, "savedstatuses");
    QString lowererStatusId = nStatusName.toLower();

    Status *savedStatus = new Status();
    acc_settings.beginGroup(lowererStatusId);
    savedStatus->Set(acc_settings.value("status",Status::FromString(lowererStatusId)).toInt(),acc_settings.value("statusName",lowererStatusId).toString());
    savedStatus->SetTitle(acc_settings.value("statusTitle",GetTooltip(lowererStatusId)).toString());
    savedStatus->SetDescription(acc_settings.value("statusDescr").toString());
    acc_settings.endGroup();
    return savedStatus;
}
