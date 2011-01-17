/*
Copyright (c) 2008-2009 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "jVCard.h"
#include "utils.h"
#include <QFile>

jVCard::jVCard(const QString &jid, bool mode, QWidget *parent) : QWidget(parent)
{
	m_mode = mode;
	m_jid = jid;
	ui_main.setupUi(this);
	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
	ui_main.widgetLayout->insertSpacerItem(0, verticalSpacer);
	QMovie *movie = new QMovie(ui_main.loaderLabel);
	movie->setFileName(SystemsCity::IconManager()->getIconPath("loading"));
	movie->start();
	ui_main.loaderLabel->setMovie(movie);
	ui_main.saveButton->setVisible(m_mode);
	ui_main.saveButton->setIcon(jPluginSystem::instance().getIcon("save_all"));
	ui_main.requestButton->setEnabled(false);
	ui_main.requestButton->setIcon(jPluginSystem::instance().getIcon("request"));
	ui_main.closeButton->setIcon(jPluginSystem::instance().getIcon("cancel"));
	connect(ui_main.saveButton, SIGNAL(clicked()), this, SLOT(saveVCard()));
	connect(ui_main.requestButton, SIGNAL(clicked()), this, SLOT(refreshVCard()));
	if (mode)
	{
		menuAddPersonal = new QMenu();
		actionUpdatePhoto = new QAction(tr("Update photo"), this);
		connect(actionUpdatePhoto, SIGNAL(triggered()), this, SLOT(getPhoto()));
		menuAddPersonal->addAction(actionUpdatePhoto);
		actionAddName = new QAction(tr("Add name"), this);
		connect(actionAddName, SIGNAL(triggered()), this, SLOT(addName()));
		menuAddPersonal->addAction(actionAddName);
		actionAddNick = new QAction(tr("Add nick"), this);
		connect(actionAddNick, SIGNAL(triggered()), this, SLOT(addNick()));
		menuAddPersonal->addAction(actionAddNick);
		actionAddBirthday = new QAction(tr("Add birthday"), this);
		connect(actionAddBirthday, SIGNAL(triggered()), this, SLOT(addBirthday()));
		menuAddPersonal->addAction(actionAddBirthday);
		actionAddUrl = new QAction(tr("Add homepage"), this);
		connect(actionAddUrl, SIGNAL(triggered()), this, SLOT(addUrl()));
		menuAddPersonal->addAction(actionAddUrl);
		actionAddDesc = new QAction(tr("Add description"), this);
		connect(actionAddDesc, SIGNAL(triggered()), this, SLOT(addDescription()));
		menuAddPersonal->addAction(actionAddDesc);
		menuAddHomeEntry = new QMenu();
		actionAddHomeCountry = new QAction(tr("Add country"), this);
		connect(actionAddHomeCountry, SIGNAL(triggered()), this, SLOT(addHomeCountry()));
		menuAddHomeEntry->addAction(actionAddHomeCountry);
		actionAddHomeRegion = new QAction(tr("Add region"), this);
		connect(actionAddHomeRegion, SIGNAL(triggered()), this, SLOT(addHomeRegion()));
		menuAddHomeEntry->addAction(actionAddHomeRegion);
		actionAddHomeCity = new QAction(tr("Add city"), this);
		connect(actionAddHomeCity, SIGNAL(triggered()), this, SLOT(addHomeCity()));
		menuAddHomeEntry->addAction(actionAddHomeCity);
		actionAddHomePostcode = new QAction(tr("Add postcode"), this);
		connect(actionAddHomePostcode, SIGNAL(triggered()), this, SLOT(addHomePostcode()));
		menuAddHomeEntry->addAction(actionAddHomePostcode);
		actionAddHomeStreet = new QAction(tr("Add street"), this);
		connect(actionAddHomeStreet, SIGNAL(triggered()), this, SLOT(addHomeStreet()));
		menuAddHomeEntry->addAction(actionAddHomeStreet);
		actionAddHomePostbox = new QAction(tr("Add PO box"), this);
		connect(actionAddHomePostbox, SIGNAL(triggered()), this, SLOT(addHomePostbox()));
		menuAddHomeEntry->addAction(actionAddHomePostbox);
		menuAddWorkEntry = new QMenu();
		actionAddWorkOrgname = new QAction(tr("Add organization name"), this);
		connect(actionAddWorkOrgname, SIGNAL(triggered()), this, SLOT(addOrgName()));
		menuAddWorkEntry->addAction(actionAddWorkOrgname);
		actionAddWorkOrgunit = new QAction(tr("Add organization unit"), this);
		connect(actionAddWorkOrgunit, SIGNAL(triggered()), this, SLOT(addOrgUnit()));
		menuAddWorkEntry->addAction(actionAddWorkOrgunit);
		actionAddWorkTitle = new QAction(tr("Add title"), this);
		connect(actionAddWorkTitle, SIGNAL(triggered()), this, SLOT(addTitle()));
		menuAddWorkEntry->addAction(actionAddWorkTitle);
		actionAddWorkRole = new QAction(tr("Add role"), this);
		connect(actionAddWorkRole, SIGNAL(triggered()), this, SLOT(addRole()));
		menuAddWorkEntry->addAction(actionAddWorkRole);
		actionAddWorkCountry = new QAction(tr("Add country"), this);
		connect(actionAddWorkCountry, SIGNAL(triggered()), this, SLOT(addWorkCountry()));
		menuAddWorkEntry->addAction(actionAddWorkCountry);
		actionAddWorkRegion = new QAction(tr("Add region"), this);
		connect(actionAddWorkRegion, SIGNAL(triggered()), this, SLOT(addWorkRegion()));
		menuAddWorkEntry->addAction(actionAddWorkRegion);
		actionAddWorkCity = new QAction(tr("Add city"), this);
		connect(actionAddWorkCity, SIGNAL(triggered()), this, SLOT(addWorkCity()));
		menuAddWorkEntry->addAction(actionAddWorkCity);
		actionAddWorkPostcode = new QAction(tr("Add postcode"), this);
		connect(actionAddWorkPostcode, SIGNAL(triggered()), this, SLOT(addWorkPostcode()));
		menuAddWorkEntry->addAction(actionAddWorkPostcode);
		actionAddWorkStreet = new QAction(tr("Add street"), this);
		connect(actionAddWorkStreet, SIGNAL(triggered()), this, SLOT(addWorkStreet()));
		menuAddWorkEntry->addAction(actionAddWorkStreet);
		actionAddWorkPostbox = new QAction(tr("Add PO box"), this);
		connect(actionAddWorkPostbox, SIGNAL(triggered()), this, SLOT(addWorkPostbox()));
		menuAddWorkEntry->addAction(actionAddWorkPostbox);
		menuEmailStatus = new QMenu();
	}
	/*
	VCard *vcard = new VCard();
	//vcard->setBday("1981-11-26");
	vcard->setNickname("reindeer");
	vcard->setRole("IPTV");
	//vcard->setUrl("http://reindeer.ru");
	setVCard(vcard, "");
	//*/
}

jVCard::~jVCard()
{
	delete m_vcard;
}

void jVCard::closeEvent(QCloseEvent *event)
{
	emit closeVCard(m_jid);
}

void jVCard::fillData()
{
	emit requestVCard(m_jid);
}

void jVCard::refreshVCard()
{
	ui_main.requestButton->setEnabled(false);
	ui_main.widgetLayout->removeItem(ui_main.widgetLayout->takeAt(0));
	delete personalWidget;
	ui_main.widgetLayout->insertSpacerItem(0, verticalSpacer);
	ui_main.loaderLabel->setVisible(true);
	emailAddresses.clear();
	phoneNumbers.clear();
	fillData();
}

void jVCard::setVCard(const VCard *vcard, const QString &avatarUrl)
{
	isAvatar = 0;
	isName = 0;
	isNick = 0;
	isBirthday = 0;
	isUrl = 0;
	isDesc = 0;
	isOrgname = 0;
	isOrgunit = 0;
	isTitle = 0;
	isRole = 0;
	isHomeCountry = 0;
	isHomeRegion = 0;
	isHomeCity = 0;
	isHomePostcode = 0;
	isHomeStreet = 0;
	isHomePostbox = 0;
	isWorkCountry = 0;
	isWorkRegion = 0;
	isWorkCity = 0;
	isWorkPostcode = 0;
	isWorkStreet = 0;
	isWorkPostbox = 0;
	ui_main.requestButton->setEnabled(true);
	ui_main.loaderLabel->setVisible(false);
	personalWidget = new QWidget();
	ui.setupUi(personalWidget);
	ui.addPersonalButton->setVisible(m_mode);
	ui.addPersonalButton->setIcon(jPluginSystem::instance().getIcon("add"));
	ui.addPersonalButton->setPopupMode(QToolButton::InstantPopup);
	ui.emailBox->setVisible(m_mode);
	ui.addEmailButton->setVisible(m_mode);
	ui.addEmailButton->setIcon(jPluginSystem::instance().getIcon("add"));
	connect(ui.addEmailButton, SIGNAL(clicked()), this, SLOT(addEmail()));
	ui.phoneBox->setVisible(m_mode);
	ui.addPhoneButton->setVisible(m_mode);
	ui.addPhoneButton->setIcon(jPluginSystem::instance().getIcon("add"));
	connect(ui.addPhoneButton, SIGNAL(clicked()), this, SLOT(addPhone()));
	ui.homeBox->setVisible(m_mode);
	ui.addHomeButton->setVisible(m_mode);
	ui.addHomeButton->setIcon(jPluginSystem::instance().getIcon("add"));
	ui.addHomeButton->setPopupMode(QToolButton::InstantPopup);
	ui.workBox->setVisible(m_mode);
	ui.addWorkButton->setVisible(m_mode);
	ui.addWorkButton->setIcon(jPluginSystem::instance().getIcon("add"));
	ui.addWorkButton->setPopupMode(QToolButton::InstantPopup);
	if (m_mode)
	{
		actionAddName->setEnabled(true);
		actionAddNick->setEnabled(true);
		actionAddBirthday->setEnabled(true);
		actionAddUrl->setEnabled(true);
		actionAddDesc->setEnabled(true);
		actionAddHomeCountry->setEnabled(true);
		actionAddHomeRegion->setEnabled(true);
		actionAddHomeCity->setEnabled(true);
		actionAddHomePostcode->setEnabled(true);
		actionAddHomeStreet->setEnabled(true);
		actionAddHomePostbox->setEnabled(true);
		actionAddWorkOrgname->setEnabled(true);
		actionAddWorkOrgunit->setEnabled(true);
		actionAddWorkTitle->setEnabled(true);
		actionAddWorkRole->setEnabled(true);
		actionAddWorkCountry->setEnabled(true);
		actionAddWorkRegion->setEnabled(true);
		actionAddWorkCity->setEnabled(true);
		actionAddWorkPostcode->setEnabled(true);
		actionAddWorkStreet->setEnabled(true);
		actionAddWorkPostbox->setEnabled(true);
		ui.addPersonalButton->setMenu(menuAddPersonal);
		ui.addHomeButton->setMenu(menuAddHomeEntry);
		ui.addWorkButton->setMenu(menuAddWorkEntry);
	}
	ui.deleteButton->setVisible(false);
	ui.deleteButton->setIcon(jPluginSystem::instance().getIcon("remove"));
	connect(ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
	ui_main.widgetLayout->removeItem(ui_main.widgetLayout->takeAt(0));
	ui_main.widgetLayout->insertWidget(0, personalWidget);
	m_vcard = (VCard*) vcard;
	avatarLabel = new VCardAvatar(m_mode);
	connect(avatarLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(avatarLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	ui.aboutLayout->insertWidget(0, avatarLabel);
	updatePhoto(avatarUrl, (avatarUrl != ""));
	if (utils::fromStd(m_vcard->formattedname()) != "")
		addName(utils::fromStd(m_vcard->formattedname()));
	if (utils::fromStd(m_vcard->nickname()) != "")
		addNick(utils::fromStd(m_vcard->nickname()));
	if (utils::fromStd(m_vcard->bday()) != "")
		addBirthday(utils::fromStd(m_vcard->bday()));
	if (utils::fromStd(m_vcard->url()) != "")
		addUrl(utils::fromStd(m_vcard->url()));
	if (!utils::fromStd(m_vcard->desc()).isEmpty())
		addDescription(utils::fromStd(m_vcard->desc()));
	if (m_vcard->emailAddresses().size())
		foreach (VCard::Email email, m_vcard->emailAddresses())
		{
			QString type;
			if (email.home)
				type = VCardConst::personalMailStatus();
			else if (email.work)
				type = VCardConst::workMailStatus();
			else
				type = VCardConst::emptyMailStatus();
			addEmail(utils::fromStd(email.userid), type);
		}
	if (m_vcard->telephone().size())
		foreach (VCard::Telephone phone, m_vcard->telephone())
		{
			QString type;
			if (phone.home)
				type = VCardConst::homePhoneStatus();
			else if (phone.work)
				type = VCardConst::workPhoneStatus();
			else if (phone.cell)
				type = VCardConst::celluarPhoneStatus();
			else
				type = VCardConst::emptyPhoneStatus();
			addPhone(utils::fromStd(phone.number), type);
		}
	if (m_vcard->addresses().size())
		foreach (VCard::Address address, m_vcard->addresses())
		{
			if (address.home)
			{
				if (utils::fromStd(address.ctry) != "")
					addHomeCountry(utils::fromStd(address.ctry));
				if (utils::fromStd(address.region) != "")
					addHomeRegion(utils::fromStd(address.region));
				if (utils::fromStd(address.locality) != "")
					addHomeCity(utils::fromStd(address.locality));
				if (utils::fromStd(address.pcode) != "")
					addHomePostcode(utils::fromStd(address.pcode));
				if (utils::fromStd(address.street) != "")
					addHomeStreet(utils::fromStd(address.street));
				if (utils::fromStd(address.pobox) != "")
					addHomePostbox(utils::fromStd(address.pobox));
			}
			if (address.work)
			{
				if (utils::fromStd(address.ctry) != "")
					addWorkCountry(utils::fromStd(address.ctry));
				if (utils::fromStd(address.region) != "")
					addWorkRegion(utils::fromStd(address.region));
				if (utils::fromStd(address.locality) != "")
					addWorkCity(utils::fromStd(address.locality));
				if (utils::fromStd(address.pcode) != "")
					addWorkPostcode(utils::fromStd(address.pcode));
				if (utils::fromStd(address.street) != "")
					addWorkStreet(utils::fromStd(address.street));
				if (utils::fromStd(address.pobox) != "")
					addWorkPostbox(utils::fromStd(address.pobox));
			}
		}
	if (utils::fromStd(m_vcard->org().name) != "")
		addOrgName(utils::fromStd(m_vcard->org().name));
	if (!m_vcard->org().units.empty() && utils::fromStd(m_vcard->org().units.front()) != "")
		addOrgUnit(utils::fromStd(m_vcard->org().units.front()));
	if (utils::fromStd(m_vcard->title()) != "")
		addTitle(utils::fromStd(m_vcard->title()));
	if (utils::fromStd(m_vcard->role()) != "")
		addRole(utils::fromStd(m_vcard->role()));
	delete m_vcard;
}

void jVCard::getPhoto()
{
	QFileDialog dialog(0,tr("Open File"),"",tr("Images (*.gif *.bmp *.jpg *.jpeg *.png)"));
	dialog.setAttribute(Qt::WA_QuitOnClose, false);
	QStringList fileList;
	if (dialog.exec())
		fileList = dialog.selectedFiles();
	if (fileList.count())
	{
		QString fileName = fileList.at(0);
		if (!fileName.isEmpty())
		{
			QFile iconFile(fileName);
			if (iconFile.size() > (64 * 1024))
				QMessageBox::warning(this, tr("Open error"), tr("Image size is too big"));
			else
				updatePhoto(fileName, true);
		}
	}
}

void jVCard::updatePhoto(const QString &file, bool state)
{
	if (state)
	{
		avatarLabel->setPhoto(file, state);
		avatarUrl = file;
		isAvatar = 1;
	}
	else
		avatarLabel->setPhoto(jPluginSystem::instance().getIconFileName("noavatar"), state);
}

void jVCard::addName(const QString &name)
{
	nameLabel = new VCardRecord(m_mode, "name");
	connect(nameLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(nameLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	QFont font;
	font.setBold(true);
	font.setWeight(75);
	nameLabel->setFont(font);
	nameLabel->setText(name);
	ui.personalLayout->insertWidget(0, nameLabel);
	isName = 1;
	if (m_mode)
		actionAddName->setEnabled(false);
}

void jVCard::addNick(const QString &nick)
{
	nickLabel = new VCardRecord(m_mode, "nick");
	connect(nickLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(nickLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	nickLabel->setText(nick);
	ui.personalLayout->insertWidget(isName, nickLabel);
	isNick = 1;
	if (m_mode)
		actionAddNick->setEnabled(false);
}

void jVCard::addBirthday(const QString &date)
{
	birthdayLabel = new VCardBirthday(m_mode);
	connect(birthdayLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(birthdayLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	birthdayLabel->setDate(date);
	ui.miscList->insertWidget(2, birthdayLabel);
	isBirthday = 1;
	if (m_mode)
		actionAddBirthday->setEnabled(false);
}

void jVCard::addUrl(const QString &url)
{
	urlLabel = new VCardRecord(m_mode, "url");
	connect(urlLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(urlLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	urlLabel->setText(url);
	ui.miscList->insertWidget(2+isBirthday, urlLabel);
	isUrl = 1;
	if (m_mode)
		actionAddUrl->setEnabled(false);
}

void jVCard::addDescription(const QString &desc)
{
	descLabel = new VCardMemo(m_mode);
	connect(descLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(descLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	descLabel->setText(desc);
	ui.miscList->addWidget(descLabel);
	isDesc = 1;
	if (m_mode)
		actionAddDesc->setEnabled(false);
}

void jVCard::addEmail(const QString &address, const QString &type)
{
	if (!ui.emailBox->isVisible())
		ui.emailBox->setVisible(true);
	VCardRecord *emailLabel = new VCardRecord(m_mode, "email");
	connect(emailLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(emailLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	emailLabel->setText(address);
	emailLabel->setStatus(type);
	ui.emailList->addWidget(emailLabel);
	emailAddresses << emailLabel;
}

void jVCard::addPhone(const QString &number, const QString &type)
{
	if (!ui.phoneBox->isVisible())
		ui.phoneBox->setVisible(true);
	VCardRecord *phoneLabel = new VCardRecord(m_mode, "phone");
	connect(phoneLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(phoneLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	phoneLabel->setText(number);
	phoneLabel->setStatus(type);
	ui.phoneList->addWidget(phoneLabel);
	phoneNumbers << phoneLabel;
}

void jVCard::addOrgName(const QString &name)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	orgnameLabel = new VCardRecord(m_mode, "orgname");
	connect(orgnameLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(orgnameLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	orgnameLabel->setText(name);
	ui.workList->insertWidget(0, orgnameLabel);
	isOrgname = 1;
	if (m_mode)
		actionAddWorkOrgname->setEnabled(false);
}

void jVCard::addOrgUnit(const QString &unit)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	orgunitLabel = new VCardRecord(m_mode, "orgunit");
	connect(orgunitLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(orgunitLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	orgunitLabel->setText(unit);
	ui.workList->insertWidget(isOrgname, orgunitLabel);
	isOrgunit = 1;
	if (m_mode)
		actionAddWorkOrgunit->setEnabled(false);
}

void jVCard::addTitle(const QString &title)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	titleLabel = new VCardRecord(m_mode, "title");
	connect(titleLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(titleLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	titleLabel->setText(title);
	ui.workList->insertWidget(isOrgname+isOrgunit, titleLabel);
	isTitle = 1;
	if (m_mode)
		actionAddWorkTitle->setEnabled(false);
}

void jVCard::addRole(const QString &role)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	roleLabel = new VCardRecord(m_mode, "role");
	connect(roleLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(roleLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	roleLabel->setText(role);
	ui.workList->insertWidget(isOrgname+isOrgunit+isTitle, roleLabel);
	isRole = 1;
	if (m_mode)
		actionAddWorkRole->setEnabled(false);
}

void jVCard::addHomeCountry(const QString &country)
{
	if (!ui.homeBox->isVisible())
		ui.homeBox->setVisible(true);
	homeCountryLabel = new VCardRecord(m_mode, "homecountry");
	connect(homeCountryLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homeCountryLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homeCountryLabel->setText(country);
	ui.homeList->insertWidget(0, homeCountryLabel);
	isHomeCountry = 1;
	if (m_mode)
		actionAddHomeCountry->setEnabled(false);
}

void jVCard::addHomeRegion(const QString &region)
{
	if (!ui.homeBox->isVisible())
		ui.homeBox->setVisible(true);
	homeRegionLabel = new VCardRecord(m_mode, "homeregion");
	connect(homeRegionLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homeRegionLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homeRegionLabel->setText(region);
	ui.homeList->insertWidget(isHomeCountry, homeRegionLabel);
	isHomeRegion = 1;
	if (m_mode)
		actionAddHomeRegion->setEnabled(false);
}

void jVCard::addHomeCity(const QString &city)
{
	if (!ui.homeBox->isVisible())
		ui.homeBox->setVisible(true);
	homeCityLabel = new VCardRecord(m_mode, "homecity");
	connect(homeCityLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homeCityLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homeCityLabel->setText(city);
	ui.homeList->insertWidget(isHomeCountry+isHomeRegion, homeCityLabel);
	isHomeCity = 1;
	if (m_mode)
		actionAddHomeCity->setEnabled(false);
}

void jVCard::addHomePostcode(const QString &pcode)
{
	if (!ui.homeBox->isVisible())
		ui.homeBox->setVisible(true);
	homePostcodeLabel = new VCardRecord(m_mode, "homepostcode");
	connect(homePostcodeLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homePostcodeLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homePostcodeLabel->setText(pcode);
	ui.homeList->insertWidget(isHomeCountry+isHomeRegion+isHomeCity, homePostcodeLabel);
	isHomePostcode = 1;
	if (m_mode)
		actionAddHomePostcode->setEnabled(false);
}

void jVCard::addHomeStreet(const QString &street)
{
	if (!ui.homeBox->isVisible())
		ui.homeBox->setVisible(true);
	homeStreetLabel = new VCardRecord(m_mode, "homestreet");
	connect(homeStreetLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homeStreetLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homeStreetLabel->setText(street);
	ui.homeList->insertWidget(isHomeCountry+isHomeRegion+isHomePostcode+isHomeCity, homeStreetLabel);
	isHomeStreet = 1;
	if (m_mode)
		actionAddHomeStreet->setEnabled(false);
}

void jVCard::addHomePostbox(const QString &pbox)
{
	if (!ui.homeBox->isVisible())
		ui.homeBox->setVisible(true);
	homePostboxLabel = new VCardRecord(m_mode, "homepostbox");
	connect(homePostboxLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homePostboxLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homePostboxLabel->setText(pbox);
	ui.homeList->insertWidget(isHomeCountry+isHomeRegion+isHomePostcode+isHomeCity+isHomeStreet, homePostboxLabel);
	isHomePostbox = 1;
	if (m_mode)
		actionAddHomePostbox->setEnabled(false);
}

void jVCard::addWorkCountry(const QString &country)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	workCountryLabel = new VCardRecord(m_mode, "workcountry");
	connect(workCountryLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workCountryLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workCountryLabel->setText(country);
	ui.workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole, workCountryLabel);
	isWorkCountry = 1;
	if (m_mode)
		actionAddWorkCountry->setEnabled(false);
}

void jVCard::addWorkRegion(const QString &region)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	workRegionLabel = new VCardRecord(m_mode, "workregion");
	connect(workRegionLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workRegionLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workRegionLabel->setText(region);
	ui.workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry, workRegionLabel);
	isWorkRegion = 1;
	if (m_mode)
		actionAddWorkRegion->setEnabled(false);
}

void jVCard::addWorkCity(const QString &city)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	workCityLabel = new VCardRecord(m_mode, "workcity");
	connect(workCityLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workCityLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workCityLabel->setText(city);
	ui.workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry+isWorkRegion, workCityLabel);
	isWorkCity = 1;
	if (m_mode)
		actionAddWorkCity->setEnabled(false);
}

void jVCard::addWorkPostcode(const QString &pcode)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	workPostcodeLabel = new VCardRecord(m_mode, "workpostcode");
	connect(workPostcodeLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workPostcodeLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workPostcodeLabel->setText(pcode);
	ui.workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry+isWorkRegion+isWorkCity, workPostcodeLabel);
	isWorkPostcode = 1;
	if (m_mode)
		actionAddWorkPostcode->setEnabled(false);
}

void jVCard::addWorkStreet(const QString &street)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	workStreetLabel = new VCardRecord(m_mode, "workstreet");
	connect(workStreetLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workStreetLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workStreetLabel->setText(street);
	ui.workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry+isWorkRegion+isWorkPostcode+isWorkCity, workStreetLabel);
	isWorkStreet = 1;
	if (m_mode)
		actionAddWorkStreet->setEnabled(false);
}

void jVCard::addWorkPostbox(const QString &pbox)
{
	if (!ui.workBox->isVisible())
		ui.workBox->setVisible(true);
	workPostboxLabel = new VCardRecord(m_mode, "workpostbox");
	connect(workPostboxLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workPostboxLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workPostboxLabel->setText(pbox);
	ui.workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry+isWorkRegion+isWorkPostcode+isWorkCity+isWorkStreet, workPostboxLabel);
	isWorkPostbox = 1;
	if (m_mode)
		actionAddWorkPostbox->setEnabled(false);
}

void jVCard::showDeleteButton()
{
	hintLayout = qobject_cast<VCardEntry*>(sender());
	hintLayout->addDeleteButton(ui.deleteButton);
}

void jVCard::hideDeleteButton()
{
	VCardEntry *layout = qobject_cast<VCardEntry*>(sender());
	if (layout == hintLayout)
		ui.deleteButton->setParent(personalWidget);
}

void jVCard::deleteEntry()
{
	ui.deleteButton->setVisible(false);
	ui.deleteButton->setParent(personalWidget);
	QString type = hintLayout->type();
	if (type != "avatar")
		delete hintLayout;
	if (type == "avatar")
	{
		updatePhoto("", false);
		isAvatar = 0;
	}
	else if (type == "name")
	{
		isName = 0;
		actionAddName->setEnabled(true);
	}
	else if (type == "nick")
	{
		isNick = 0;
		actionAddNick->setEnabled(true);
	}
	else if (type == "birthday")
	{
		isBirthday = 0;
		actionAddBirthday->setEnabled(true);
	}
	else if (type == "url")
	{
		isUrl = 0;
		actionAddUrl->setEnabled(true);
	}
	else if (type == "memo")
	{
		isDesc = 0;
		actionAddDesc->setEnabled(true);
	}
	else if (type == "email")
	{
		emailAddresses.removeOne((VCardRecord*) hintLayout);
	}
	else if (type == "phone")
	{
		phoneNumbers.removeOne((VCardRecord*) hintLayout);
	}
	else if (type == "orgname")
	{
		isOrgname = 0;
		actionAddWorkOrgname->setEnabled(true);
	}
	else if (type == "orgunit")
	{
		isOrgunit = 0;
		actionAddWorkOrgunit->setEnabled(true);
	}
	else if (type == "title")
	{
		isTitle = 0;
		actionAddWorkTitle->setEnabled(true);
	}
	else if (type == "role")
	{
		isRole = 0;
		actionAddWorkRole->setEnabled(true);
	}
	else if (type == "workcountry")
	{
		isWorkCountry = 0;
		actionAddWorkCountry->setEnabled(true);
	}
	else if (type == "workregion")
	{
		isWorkRegion = 0;
		actionAddWorkRegion->setEnabled(true);
	}
	else if (type == "workcity")
	{
		isWorkCity = 0;
		actionAddWorkCity->setEnabled(true);
	}
	else if (type == "workpostcode")
	{
		isWorkPostcode = 0;
		actionAddWorkPostcode->setEnabled(true);
	}
	else if (type == "workstreet")
	{
		isWorkStreet = 0;
		actionAddWorkStreet->setEnabled(true);
	}
	else if (type == "workpostbox")
	{
		isWorkPostbox = 0;
		actionAddWorkPostbox->setEnabled(true);
	}
	else if (type == "homecountry")
	{
		isHomeCountry = 0;
		actionAddHomeCountry->setEnabled(true);
	}
	else if (type == "homeregion")
	{
		isHomeRegion = 0;
		actionAddHomeRegion->setEnabled(true);
	}
	else if (type == "homecity")
	{
		isHomeCity = 0;
		actionAddHomeCity->setEnabled(true);
	}
	else if (type == "homepostcode")
	{
		isHomePostcode = 0;
		actionAddHomePostcode->setEnabled(true);
	}
	else if (type == "homestreet")
	{
		isHomeStreet = 0;
		actionAddHomeStreet->setEnabled(true);
	}
	else if (type == "homepostbox")
	{
		isHomePostbox = 0;
		actionAddHomePostbox->setEnabled(true);
	}
}

void jVCard::saveVCard()
{
	VCard *vcard = new VCard();
	if (isAvatar)
	{
		QFile file(avatarUrl);
		if(file.open(QIODevice::ReadOnly))
		{
			QByteArray data = file.readAll();
			const std::string std_data (data.data(),data.size());
			file.close();
			vcard->setPhoto(utils::toStd("undefined"), std_data);
		}
	}
	if (isName && nameLabel->text() != VCardConst::emptyString())
		vcard->setFormattedname(utils::toStd(nameLabel->text()));
	if (isNick && nickLabel->text() != VCardConst::emptyString())
		vcard->setNickname(utils::toStd(nickLabel->text()));
	if (isBirthday && birthdayLabel->date() != VCardConst::emptyString())
		vcard->setBday(utils::toStd(birthdayLabel->date()));
	if (isUrl && urlLabel->text() != VCardConst::emptyString())
		vcard->setUrl(utils::toStd(urlLabel->text()));
	if (isDesc && descLabel->text() != VCardConst::emptyString())
		vcard->setDesc(utils::toStd(descLabel->text()));
	int emailCount = emailAddresses.count();
	if (emailCount)
	{
		for (int emailNum = 0; emailNum < emailCount; emailNum++)
			if (emailAddresses[emailNum] && emailAddresses[emailNum]->text() != VCardConst::emptyString())
			{
				QString email = emailAddresses[emailNum]->text();
				QString status = emailAddresses[emailNum]->status();
				int type;
				if (status == VCardConst::personalMailStatus())
					type = VCard::AddrTypeHome;
				else if (status == VCardConst::workMailStatus())
					type = VCard::AddrTypeWork;
				else
					type = 0;
				vcard->addEmail(utils::toStd(email), type);
			}
	}
	int phoneCount = phoneNumbers.count();
	if (phoneCount)
	{
		for (int phoneNum = 0; phoneNum < phoneCount; phoneNum++)
			if (phoneNumbers[phoneNum] && phoneNumbers[phoneNum]->text() != VCardConst::emptyString())
			{
				QString phone = phoneNumbers[phoneNum]->text();
				QString status = phoneNumbers[phoneNum]->status();
				int type;
				if (status == VCardConst::homePhoneStatus())
					type = VCard::AddrTypeHome;
				else if (status == VCardConst::workPhoneStatus())
					type = VCard::AddrTypeWork;
				else if (status == VCardConst::celluarPhoneStatus())
					type = VCard::AddrTypeCell;
				else
					type = 0;
				vcard->addTelephone(utils::toStd(phone), type);
			}
	}
	if (isHomeCountry+isHomeRegion+isHomeCity+isHomePostcode+isHomeStreet+isHomePostbox)
	{
		QString country;
		QString region = "";
		QString city = "";
		QString pcode = "";
		QString street = "";
		QString pbox = "";
		if (isHomeCountry && homeCountryLabel->text() != VCardConst::emptyString())
			country = homeCountryLabel->text();
		if (isHomeRegion && homeRegionLabel->text() != VCardConst::emptyString())
			region = homeRegionLabel->text();
		if (isHomeCity && homeCityLabel->text() != VCardConst::emptyString())
			city = homeCityLabel->text();
		if (isHomePostcode && homePostcodeLabel->text() != VCardConst::emptyString())
			pcode = homePostcodeLabel->text();
		if (isHomeStreet && homeStreetLabel->text() != VCardConst::emptyString())
			street = homeStreetLabel->text();
		if (isHomePostbox && homePostboxLabel->text() != VCardConst::emptyString())
			pbox = homePostboxLabel->text();
		vcard->addAddress(utils::toStd(pbox), utils::toStd(""), utils::toStd(street), utils::toStd(city), utils::toStd(region), utils::toStd(pcode), utils::toStd(country), VCard::AddrTypeHome);
	}
	if (isWorkCountry+isWorkRegion+isWorkCity+isWorkPostcode+isWorkStreet+isWorkPostbox)
	{
		QString country = "";
		QString region = "";
		QString city = "";
		QString pcode = "";
		QString street = "";
		QString pbox = "";
		if (isWorkCountry && workCountryLabel->text() != VCardConst::emptyString())
			country = workCountryLabel->text();
		if (isWorkRegion && workRegionLabel->text() != VCardConst::emptyString())
			region = workRegionLabel->text();
		if (isWorkCity && workCityLabel->text() != VCardConst::emptyString())
			city = workCityLabel->text();
		if (isWorkPostcode && workPostcodeLabel->text() != VCardConst::emptyString())
			pcode = workPostcodeLabel->text();
		if (isWorkStreet && workStreetLabel->text() != VCardConst::emptyString())
			street = workStreetLabel->text();
		if (isWorkPostbox && workPostboxLabel->text() != VCardConst::emptyString())
			pbox = workPostboxLabel->text();
		vcard->addAddress(utils::toStd(pbox), utils::toStd(""), utils::toStd(street), utils::toStd(city), utils::toStd(region), utils::toStd(pcode), utils::toStd(country), VCard::AddrTypeWork);
	}
	if (isOrgname+isOrgunit)
	{
		QString name = "";
		if (isOrgname && orgnameLabel->text() != VCardConst::emptyString())
			name = orgnameLabel->text();
		gloox::StringList units;
		if (isOrgunit && orgunitLabel->text() != VCardConst::emptyString())
			units.assign(1, utils::toStd(orgunitLabel->text()));
		vcard->setOrganization(utils::toStd(name), units);
	}
	if (isTitle && titleLabel->text() != VCardConst::emptyString())
		vcard->setTitle(utils::toStd(titleLabel->text()));
	if (isRole && roleLabel->text() != VCardConst::emptyString())
		vcard->setRole(utils::toStd(roleLabel->text()));
	emit saveVCard(vcard);
}
