#include "twstatusobject.h"
#include "twpluginsystem.h"

twStatusObject::twStatusObject(const QString &account_name, const QString &profile_name,
					 twApiWrap *api_wrapper, QObject *parent)
	: QObject(parent),
	m_api_wrapper(api_wrapper),
	m_profile_name(profile_name),
	m_account_name(account_name)
{
    m_status_menu = new QMenu;
    m_status_menu->setTitle(account_name);
		m_online_icon = QIcon(":/icons/twitter-online.png");
		m_offline_icon = QIcon(":/icons/twitter-offline.png");
		m_connecting_icon = QIcon(":/icons/twitter-offline.png");
    m_status_menu->setIcon(m_offline_icon);
    m_online_action = new QAction(m_online_icon,tr("Online"),this);
    m_offline_action = new QAction(m_offline_icon,tr("Offline"),this);
    m_online_action->setCheckable(true);
    m_offline_action->setCheckable(true);
    m_offline_action->setChecked(true);
    m_account_button = new QToolButton;
    m_account_button->setMinimumSize(QSize(22, 22));
    m_account_button->setMaximumSize(QSize(22, 22));
    m_account_button->setAutoRaise(true);
    m_account_button->setIcon(m_offline_icon);
    m_account_button->setToolTip(account_name);
    m_status_menu->addAction(m_online_action);
    m_status_menu->addAction(m_offline_action);

    m_current_connected = false;

    connect(m_online_action, SIGNAL(triggered()), this, SLOT(connectToServer()));
    connect(m_offline_action, SIGNAL(triggered()), this, SLOT(disconnectFromServer()));
		connect(m_api_wrapper, SIGNAL(iMDisconnected()), this, SLOT(wrapperDisconnected()));
		connect(m_api_wrapper, SIGNAL(iMConnected()), this, SLOT(wrapperConnected()));

    loadSettings();
}

twStatusObject::~twStatusObject()
{
    delete m_online_action;
    delete m_offline_action;
    delete m_status_menu;
    delete m_account_button;
}

void twStatusObject::loadSettings()
{
		QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/twitter."+m_account_name, "accountsettings");
    if (settings.value("main/autoconnect",false).toBool() )
	connectToServer();
}

void twStatusObject::addButtonToLayout(QHBoxLayout *layout)
{

#if defined(Q_OS_MAC)
        layout->addWidget(accountLineButton, 0, Qt::AlignLeft);
#else
        layout->addWidget(m_account_button, 0, Qt::AlignRight);
#endif

        m_account_button->setPopupMode(QToolButton::InstantPopup);
        m_account_button->setMenu(m_status_menu);
}

QIcon twStatusObject::getCurrentIcon()
{
    return m_account_button->icon();
}

void twStatusObject::connectToServer()
{
    if ( !m_current_connected )
    {
	m_current_connected = true;
	m_account_button->setIcon(m_connecting_icon);
	m_status_menu->setIcon(m_connecting_icon);
	twPluginSystem::instance().updateStatusIcons();
	m_api_wrapper->startRequests();
    }
}

void twStatusObject::disconnectFromServer()
{
	if (m_current_connected) {
		m_api_wrapper->stopAll();
		wrapperDisconnected();
	}
}

void twStatusObject::wrapperConnected()
{
	m_online_action->setChecked(true);
	m_offline_action->setChecked(false);
	m_account_button->setIcon(m_online_icon);
	m_status_menu->setIcon(m_online_icon);
	twPluginSystem::instance().updateStatusIcons();
	twPluginSystem::instance().setAccountIsOnline(m_account_name,true);
}

void twStatusObject::wrapperDisconnected()
{

	m_current_connected = false;
	m_online_action->setChecked(false);
	m_offline_action->setChecked(true);
	m_account_button->setIcon(m_offline_icon);
	m_status_menu->setIcon(m_offline_icon);
	twPluginSystem::instance().updateStatusIcons();
	twPluginSystem::instance().setAccountIsOnline(m_account_name,false);
	emit iMOffline();
}
