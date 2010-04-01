#include "xtrazsettings.h"
#include "xtrazutils.h"
#include "ui_xtrazsettings.h"
#include <QSettings>
#include <QListWidgetItem>
#include <qdebug.h>

XtrazSettings::XtrazSettings(QString aProfileName, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::XtrazSettings),
    m_profileName(aProfileName)
{
    m_ui->setupUi(this);
    QList<XtrazPackage*> packages = XtrazUtils::FindPackages();
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName, "xtrazsettings");
    QString current = settings.value("currentPack/path").toString();
    bool enabled = settings.value("main/enabled",true).toBool();
    m_ui->enabledCheck->setChecked( (enabled) ? Qt::Checked : Qt::Unchecked );
    foreach (XtrazPackage* pack, packages)
    {
        qDebug()<<"Xtraz settings: adding package to list: "<<pack->Name();
        QListWidgetItem* item = new QListWidgetItem(pack->Name());
        item->setData(Qt::UserRole,pack->Path());
        m_ui->packagesListView->addItem(item);
        if (!current.isEmpty() && current == pack->Path())
        {
            m_ui->packagesListView->setCurrentItem(item);
        }
    }
}

XtrazSettings::~XtrazSettings()
{
    delete m_ui;
}

void XtrazSettings::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void XtrazSettings::SaveSettings()
{
    QListWidgetItem* current = m_ui->packagesListView->currentItem();
    if (!current) return;
    QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profileName, "xtrazsettings");
    settings.setValue("currentPack/path",current->data(Qt::UserRole));
    bool enabled = (m_ui->enabledCheck->checkState() == Qt::Checked)? true : false;
    settings.setValue("main/enabled",enabled);
}
