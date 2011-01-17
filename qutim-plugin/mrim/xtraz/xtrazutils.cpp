#include "xtrazutils.h"
#include <QSettings>
#include <QStringList>
#include <QApplication>
#include <QDir>
#include <qdebug.h>

XtrazUtils::XtrazUtils()
{
}

QList<XtrazPackage*> XtrazUtils::FindPackages()
{
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim", "qutimsettings");

    QStringList paths;
    QString appPath = qApp->applicationDirPath();
    if (appPath.at(appPath.size()-1) == '/') appPath.chop(1);
#if !defined(Q_WS_WIN)
    // ../share/qutim
    paths << appPath.section('/', 0, -2) + "/share/qutim";
#endif
    // ./
    paths << appPath;
    // ~/.config/qutim or Application Data
    paths << settings.fileName().section('/', 0, -2);
#if defined(Q_WS_MAC)
    // ./ Application bundle
    paths << qApp->applicationDirPath().section('/', 0, -2) + "/Resources";
#endif
    QList<XtrazPackage*> packagesList;
    foreach (QString path, paths)
    {
        QString packsPath = path+"/xtrazpacks";
        packagesList.append(ProccessPackages(packsPath));
    }
    qDebug()<<"Packages total count: "<<packagesList.count();
    return packagesList;
}

QList<XtrazPackage*> XtrazUtils::ProccessPackages(const QString& aPath)
{
    qDebug()<<"Searching for Xtraz package in: "<<aPath;
    QDir xtrazPacksDir(aPath);
    QStringList xtrazPacks(xtrazPacksDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

    QList<XtrazPackage*> packagesList;
    foreach (QString dir, xtrazPacks)
    {
        QString fullPath = aPath + "/" + dir;
        qDebug()<<"Checking Xtraz package in: "<<fullPath;
        XtrazPackage* package = XtrazPackage::TryParse(fullPath+"/xtrazpack.xml");
        if (!package)
        {
            qDebug()<<"No Xtraz packages in this directory!";
            continue;
        }
        qDebug()<<"Xtraz package is valid!";
        packagesList.append(package);
    }
    qDebug()<<"Packages total count: "<<packagesList.count();
    return packagesList;
}
