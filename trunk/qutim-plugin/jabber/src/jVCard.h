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

#ifndef JVCARD_H
#define JVCARD_H

#include <QtGui>
#include "ui_jVCard.h"
#include "ui_wPersonal.h"

#include <gloox/vcard.h>
#include <qutim/iconmanagerinterface.h>
#include "jPluginSystem.h"
#include "VCardRecord.h"
#include "VCardBirthday.h"
#include "VCardAvatar.h"
#include "VCardConst.h"
#include "VCardMemo.h"
using namespace gloox;

class VCardRecord;

class jVCard : public QWidget
{
	Q_OBJECT

	public:
		jVCard(const QString &jid, bool mode, QWidget *parent = 0);
		~jVCard();
		void setVCard(const VCard *vcard, const QString &avatarUrl);
		void fillData();

	signals:
		void saveVCard(VCard *vcard);
		void requestVCard(const QString &jid);
		void closeVCard(const QString &jid);

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void saveVCard();
		void refreshVCard();
		void getPhoto();
		void addName() {addName(VCardConst::emptyString());}
		void addNick() {addNick(VCardConst::emptyString());}
		void addBirthday() {addBirthday(VCardConst::emptyString());}
		void addUrl() {addUrl(VCardConst::emptyString());}
		void addDescription() {addDescription(VCardConst::emptyString());}
		void addEmail() {addEmail(VCardConst::emptyString(), VCardConst::emptyMailStatus());}
		void addPhone() {addPhone(VCardConst::emptyString(), VCardConst::emptyPhoneStatus());}
		void addOrgName() {addOrgName(VCardConst::emptyString());}
		void addOrgUnit() {addOrgUnit(VCardConst::emptyString());}
		void addTitle() {addTitle(VCardConst::emptyString());}
		void addRole() {addRole(VCardConst::emptyString());}
		void addHomeCountry() {addHomeCountry(VCardConst::emptyString());}
		void addHomeRegion() {addHomeRegion(VCardConst::emptyString());}
		void addHomePostcode() {addHomePostcode(VCardConst::emptyString());}
		void addHomeCity() {addHomeCity(VCardConst::emptyString());}
		void addHomeStreet() {addHomeStreet(VCardConst::emptyString());}
		void addHomePostbox() {addHomePostbox(VCardConst::emptyString());}
		void addWorkCountry() {addWorkCountry(VCardConst::emptyString());}
		void addWorkRegion() {addWorkRegion(VCardConst::emptyString());}
		void addWorkCity() {addWorkCity(VCardConst::emptyString());}
		void addWorkPostcode() {addWorkPostcode(VCardConst::emptyString());}
		void addWorkStreet() {addWorkStreet(VCardConst::emptyString());}
		void addWorkPostbox() {addWorkPostbox(VCardConst::emptyString());}
		void showDeleteButton();
		void hideDeleteButton();
		void deleteEntry();

	private:
		void updatePhoto(const QString &file, bool state);
		void addName(const QString &name);
		void addNick(const QString &nick);
		void addBirthday(const QString &date);
		void addUrl(const QString &url);
		void addDescription(const QString &desc);
		void addEmail(const QString &address, const QString &type);
		void addPhone(const QString &number, const QString &type);
		VCardRecord* addAddressEntry(const QString &entry, QVBoxLayout *parent, VCardRecord *label, int layer);
		void addOrgName(const QString &name);
		void addOrgUnit(const QString &unit);
		void addTitle(const QString &title);
		void addRole(const QString &role);
		void addHomeCountry(const QString &county);
		void addHomeRegion(const QString &region);
		void addHomePostcode(const QString &pcode);
		void addHomeCity(const QString &city);
		void addHomeStreet(const QString &street);
		void addHomePostbox(const QString &pbox);
		void addWorkCountry(const QString &county);
		void addWorkRegion(const QString &region);
		void addWorkPostcode(const QString &pcode);
		void addWorkCity(const QString &city);
		void addWorkStreet(const QString &street);
		void addWorkPostbox(const QString &pbox);
		QString avatarUrl;
		bool m_mode;
		QString m_jid;
		QWidget *personalWidget;
		Ui::jVCard ui_main;
		Ui::Personal ui;
		VCard *m_vcard;
		VCardAvatar *avatarLabel;
		VCardRecord *nameLabel;
		VCardRecord *nickLabel;
		VCardBirthday *birthdayLabel;
		VCardRecord *urlLabel;
		VCardMemo *descLabel;
		QList<VCardRecord*> emailAddresses;
		QList<VCardRecord*> phoneNumbers;
		VCardRecord *homeCountryLabel;
		VCardRecord *homeRegionLabel;
		VCardRecord *homeCityLabel;
		VCardRecord *homePostcodeLabel;
		VCardRecord *homeStreetLabel;
		VCardRecord *homePostboxLabel;
		VCardRecord *workCountryLabel;
		VCardRecord *workRegionLabel;
		VCardRecord *workCityLabel;
		VCardRecord *workPostcodeLabel;
		VCardRecord *workStreetLabel;
		VCardRecord *workPostboxLabel;
		VCardRecord *orgnameLabel;
		VCardRecord *orgunitLabel;
		VCardRecord *titleLabel;
		VCardRecord *roleLabel;
		int isAvatar, isName, isNick, isBirthday, isUrl, isDesc;
		int isOrgname, isOrgunit, isTitle, isRole, isWorkCountry, isWorkRegion, isWorkCity, isWorkPostcode, isWorkStreet, isWorkPostbox;
		int isHomeCountry, isHomeRegion, isHomeCity, isHomePostcode, isHomeStreet, isHomePostbox;
		QMenu *menuAddPersonal;
		QAction *actionUpdatePhoto;
		QAction *actionAddName;
		QAction *actionAddNick;
		QAction *actionAddBirthday;
		QAction *actionAddUrl;
		QAction *actionAddDesc;
		QMenu *menuAddHomeEntry;
		QAction *actionAddHomeCountry;
		QAction *actionAddHomeRegion;
		QAction *actionAddHomeCity;
		QAction *actionAddHomePostcode;
		QAction *actionAddHomeStreet;
		QAction *actionAddHomePostbox;
		QMenu *menuAddWorkEntry;
		QAction *actionAddWorkOrgname;
		QAction *actionAddWorkOrgunit;
		QAction *actionAddWorkTitle;
		QAction *actionAddWorkRole;
		QAction *actionAddWorkCountry;
		QAction *actionAddWorkRegion;
		QAction *actionAddWorkCity;
		QAction *actionAddWorkPostcode;
		QAction *actionAddWorkStreet;
		QAction *actionAddWorkPostbox;
		QMenu *menuEmailStatus;
		QAction *actionEmailPersonal;
		QAction *actionEmailWork;
		QAction *actionEmailEmpty;
		QSpacerItem *verticalSpacer;
		VCardEntry *hintLayout;
};

#endif // JVCARD_H
