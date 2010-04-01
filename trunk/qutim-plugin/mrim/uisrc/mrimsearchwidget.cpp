/*****************************************************************************
    MRIMSearchWidget

    Copyright (c) 2009 by Rusanov Peter <tazkrut@mail.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "mrimsearchwidget.h"
#include "../coresrc/RegionListParser.h"
#include "../coresrc/MRIMUtils.h"
#include "../coresrc/MRIMClient.h"

MRIMSearchWidget::MRIMSearchWidget(MRIMClient* aClient, QWidget *parent)
        : QWidget(parent), m_client(aClient)
{
	ui.setupUi(this);
        move(MRIMCommonUtils::DesktopCenter(size()));

	QRegExp rxMail("([a-zA-Z0-9\\-\\_\\.]+@([a-zA-Z0-9\\-\\_]+\\.)+[a-zA-Z]+)");
	QValidator *validatorMail = new QRegExpValidator(rxMail, this);
	ui.emailEdit->setValidator(validatorMail); 

	m_listParser = new RegionListParser(":/Resources/region.txt");
	m_regionsList = m_listParser->GetRegionsList();
	ui.countryComboBox->addItem(tr("Any"),-1);
	if (m_regionsList != NULL)
	{
		foreach (LiveRegion reg,*m_regionsList)
		{
			int existIndex = ui.countryComboBox->findData(reg.countryId);
			if (reg.cityId == 0 && existIndex == -1)
			{
				ui.countryComboBox->addItem(reg.name,reg.countryId);
			}
		}		
		on_countryComboBox_currentIndexChanged(0);
	}

	ui.zodiacComboBox->addItem(tr("Any"),-1);
	ui.zodiacComboBox->addItem(tr("The Ram"),1);
	ui.zodiacComboBox->addItem(tr("The Bull"),2);
	ui.zodiacComboBox->addItem(tr("The Twins"),3);
	ui.zodiacComboBox->addItem(tr("The Crab"),4);
	ui.zodiacComboBox->addItem(tr("The Lion"),5);
	ui.zodiacComboBox->addItem(tr("The Virgin"),6);
	ui.zodiacComboBox->addItem(tr("The Balance"),7);
	ui.zodiacComboBox->addItem(tr("The Scorpion"),8);
	ui.zodiacComboBox->addItem(tr("The Archer"),9);
	ui.zodiacComboBox->addItem(tr("The Capricorn"),10);
	ui.zodiacComboBox->addItem(tr("The Water-bearer"),11);
	ui.zodiacComboBox->addItem(tr("The Fish"),12);

	ui.sexComboBox->addItem(tr("Any"),-1);
	ui.sexComboBox->addItem(tr("Male"),1);
	ui.sexComboBox->addItem(tr("Female"),2);

	ui.birthMonthComboBox->addItem(tr("Any"),-1);
	ui.birthMonthComboBox->addItem(tr("January"),1);
	ui.birthMonthComboBox->addItem(tr("February"),2);
	ui.birthMonthComboBox->addItem(tr("March"),3);
	ui.birthMonthComboBox->addItem(tr("April"),4);
	ui.birthMonthComboBox->addItem(tr("May"),5);
	ui.birthMonthComboBox->addItem(tr("June"),6);
	ui.birthMonthComboBox->addItem(tr("July"),7);
	ui.birthMonthComboBox->addItem(tr("August"),8);
	ui.birthMonthComboBox->addItem(tr("September"),9);
	ui.birthMonthComboBox->addItem(tr("October"),10);
	ui.birthMonthComboBox->addItem(tr("November"),11);
	ui.birthMonthComboBox->addItem(tr("December"),12);
}

MRIMSearchWidget::~MRIMSearchWidget()
{

}


void MRIMSearchWidget::on_groupBox_toggled(bool)
{

}

void MRIMSearchWidget::on_emailEdit_textChanged(QString)
{
	if (ui.emailEdit->text().length() > 0)
	{
		ui.searchGroupBox->setEnabled(false);
	}
	else
	{
		ui.searchGroupBox->setEnabled(true);
	}
}

void MRIMSearchWidget::on_countryComboBox_currentIndexChanged(int aNewIndex)
{
	ui.regionComboBox->clear();
	bool isOk;
        quint32 currCountryId = ui.countryComboBox->itemData(aNewIndex).toUInt(&isOk);
	if (isOk)
	{
		if (m_regionsList != NULL)
		{
                        qint32 addedCount = 0;
			foreach (LiveRegion reg,*m_regionsList)
			{
                                qint32 existIndex = ui.regionComboBox->findData(reg.cityId);
				if (reg.countryId == currCountryId && reg.cityId != 0 && existIndex == -1)
				{
					ui.regionComboBox->addItem(reg.name,reg.cityId);
					addedCount++;
				}
			}		
			if (addedCount == 0)
			{
				ui.regionComboBox->addItem(tr("Any"),-1);
			}
		}
	}	
}

void MRIMSearchWidget::on_pushButton_clicked()
{
	MRIMSearchParams searchParams;
	QStringList email = ui.emailEdit->text().split("@");
	if (email.count() > 1)
	{
		searchParams.EmailAddr = email[0];
		searchParams.EmailDomain = email[1];
		searchParams.Nick = "";
		searchParams.Name = "";
		searchParams.Surname = "";
		searchParams.Sex = -1;
		searchParams.MinAge = -1;
		searchParams.MaxAge = -1;
		searchParams.ZodiacId = -1;
		searchParams.CountryId = -1;
		searchParams.CityId = -1;
		searchParams.BirthDay = -1;
		searchParams.BirthdayMonth = -1;
		searchParams.OnlineOnly = false;
	}
	else
	{	
		searchParams.EmailAddr = "";
		searchParams.EmailDomain = "";

		searchParams.Nick = ui.nickEdit->text();
		searchParams.Name = ui.nameEdit->text();
		searchParams.Surname = ui.surnameEdit->text();
		searchParams.Sex = ui.sexComboBox->itemData(ui.sexComboBox->currentIndex()).toInt();

		bool ok = false;
		if (ui.ageFromEdit->text().length() > 0)
		{
			searchParams.MinAge = ui.ageFromEdit->text().toInt(&ok);
		}

		if (!ok)
		{
			searchParams.MinAge = -1;
		}

		ok = false;
		if (ui.ageToEdit->text().length() > 0)
		{
			searchParams.MaxAge = ui.ageToEdit->text().toInt(&ok);
		}

		if (!ok)
		{
			searchParams.MaxAge = -1;
		}
		
		ok = false;
		searchParams.ZodiacId = ui.zodiacComboBox->itemData(ui.zodiacComboBox->currentIndex()).toInt(&ok);
		if (!ok)
		{
			searchParams.ZodiacId = -1;
		}

		ok = false;
		searchParams.CountryId = ui.countryComboBox->itemData(ui.countryComboBox->currentIndex()).toInt(&ok);
		if (!ok)
		{
			searchParams.CountryId = -1;
		}

		ok = false;
		searchParams.CityId = ui.regionComboBox->itemData(ui.regionComboBox->currentIndex()).toInt(&ok);
		if (!ok)
		{
			searchParams.CityId = -1;
		}

		ok = false;
		searchParams.BirthDay = ui.birthDayComboBox->itemText(ui.birthDayComboBox->currentIndex()).toInt(&ok);
		if (!ok)
		{
			searchParams.BirthDay = -1;
		}

		ok = false;
		searchParams.BirthdayMonth = ui.birthMonthComboBox->itemData(ui.birthMonthComboBox->currentIndex()).toInt(&ok);
		if (!ok)
		{
			searchParams.BirthdayMonth = -1;
		}

		searchParams.OnlineOnly = (ui.onlyOnlineCheckBox->checkState() == Qt::Checked);
	}
        m_client->Protocol()->StartSearch(searchParams);
	setEnabled(false);	
}

void MRIMSearchWidget::SearchFinished(quint32 aFoundCnts)
{
	setEnabled(true);
	if (aFoundCnts > 0)
	{
                hide();
	}
}

bool MRIMSearchWidget::ShowAvatars()
{
    return ui.showAvatarsCheckBox->checkState() == Qt::Checked;
}
