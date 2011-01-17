#include "editaccount.h"
#include "ui_editaccount.h"
#include "loginform.h"
#include "settingswidget.h"
#include "../coresrc/MRIMClient.h"
#include "../coresrc/MRIMUtils.h"

EditAccount::EditAccount(MRIMClient* aClient, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::EditAccount),
    m_client(aClient)
{
    m_loginWidget = new LoginForm(m_client);
    m_settingsWidget = new SettingsWidget(m_client->ProfileName(),m_client->AccountName());
    m_ui->setupUi(this);    
    move(MRIMCommonUtils::DesktopCenter(size()));

    setWindowTitle(tr("Edit %1 account settings").arg(m_client->AccountName()));
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/icons/core/apply.png"));
    m_ui->buttonBox->button(QDialogButtonBox::Apply)->setIcon(QIcon(":/icons/core/apply.png"));
    m_ui->buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/icons/core/cancel.png"));
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_ui->accountTab->layout()->setAlignment(Qt::AlignTop);
    m_ui->accountTab->layout()->addWidget(m_loginWidget);

    m_ui->connectionTab->layout()->setAlignment(Qt::AlignTop);
    m_ui->connectionTab->layout()->addWidget(m_settingsWidget);

    m_loginWidget->LoadSettings();
    QSettings accountSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_client->ProfileName()+"/mrim."+m_client->AccountName(),"accountsettings");
    bool profileDefaults = accountSettings.value("main/useProfileDefaults").toBool();
    m_ui->useProfileCheckBox->setCheckState( (profileDefaults) ? Qt::Checked : Qt::Unchecked);
    m_settingsWidget->setEnabled(!profileDefaults);
}

EditAccount::~EditAccount()
{
    delete m_ui;
    delete m_loginWidget;
    delete m_settingsWidget;
}

void EditAccount::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void EditAccount::on_buttonBox_accepted()
{
    SaveSettings();
    close();
}

void EditAccount::on_buttonBox_rejected()
{
    close();
}

void EditAccount::on_useProfileCheckBox_clicked()
{
    m_settingsWidget->setEnabled(m_ui->useProfileCheckBox->checkState() == Qt::Unchecked);
}

void EditAccount::on_buttonBox_clicked(QAbstractButton* button)
{
    if (button == m_ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        SaveSettings();
    }
}

void EditAccount::SaveSettings()
{
    m_loginWidget->SaveSettings();
    m_settingsWidget->SaveSettings();

    QSettings accountSettings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_client->ProfileName()+"/mrim."+m_client->AccountName(),"accountsettings");
    accountSettings.setValue("main/useProfileDefaults",m_ui->useProfileCheckBox->checkState() == Qt::Checked);
    m_client->UpdateSettings();
}
