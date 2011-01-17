/*
    weatherSettings.cpp

    Copyright (c) 2008 by Nikita 'Δz' Belov <null@deltaz.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "weatherSettings.h"

weatherSettings::weatherSettings(QString profileName) : m_profile_name(profileName)
{
	ui.setupUi(this);

	QSettings settings(QSettings::defaultFormat(),QSettings::UserScope, "qutim/qutim."+m_profile_name, "weatherPlugin");
	ui.intervalBox->setValue(settings.value("main/interval", 25).toInt());
	ui.showStatusBox->setCheckState((settings.value("main/showstatus", 2).toInt()==2?Qt::Checked:Qt::Unchecked));
	int count = settings.value("main/countItems", 0).toInt();
	for (int i=0; i<count; i++)
		ui.cityList->addItem(settings.value("main/item_"+QString::number(i)).toString());

	netMan = new QNetworkAccessManager();
	connect(netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(searchFinished(QNetworkReply*)));

	connect(ui.searchList, SIGNAL(currentRowChanged(int)), this, SLOT(searchListRowChanged(int)));
	connect(ui.searchList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(searchListItemDoubleClicked(QListWidgetItem *)));
}

weatherSettings::~weatherSettings()
{
	netMan->disconnect();
	delete netMan;
}

void weatherSettings::saveSettings()
{
	QSettings settings(QSettings::defaultFormat(),QSettings::UserScope, "qutim/qutim."+m_profile_name, "weatherPlugin");
	settings.setValue("main/interval", ui.intervalBox->value());
	settings.setValue("main/showstatus", ui.showStatusBox->checkState());
	settings.setValue("main/countItems", ui.cityList->count());
	for (int i=0; i<ui.cityList->count(); i++)
		settings.setValue("main/item_"+QString::number(i), ui.cityList->item(i)->text());

}

void weatherSettings::on_addCityButton_clicked()
{
	ui.cityList->addItem(ui.cityEdit->text());
	ui.cityEdit->setText("");
}

void weatherSettings::on_deleteCityButton_clicked()
{
	int row = ui.cityList->currentRow();
	ui.cityList->takeItem(row);
	delete ui.cityList->item(row);
}

void weatherSettings::on_searchButton_clicked()
{
	netMan->get(QNetworkRequest(QUrl(QString("http://xoap.weather.com/search/search?where=%1").arg(ui.searchEdit->text()))));
	ui.searchResult->setText("Searching...");
}

void weatherSettings::searchFinished(QNetworkReply *reply)
{
	for (int i=0; i < searchResults.count(); i++)
	{
		ui.searchList->takeItem(0);
		delete ui.searchList->item(0);
	}
	searchResults.clear();

	QDomDocument doc;
	if(doc.setContent(reply->readAll()))
	{
		QDomElement rootElement = doc.documentElement();
		int itemsCount = rootElement.elementsByTagName("loc").count();
		if (itemsCount == 0)
			ui.searchResult->setText("Not found");
		else
			ui.searchResult->setText(QString("Found: %1").arg(QString::number(itemsCount)));

		for (int i=0; i < itemsCount; i++)
		{
			QString cityId = rootElement.elementsByTagName("loc").at(i).attributes().namedItem("id").nodeValue();
			QString cityName = rootElement.elementsByTagName("loc").at(i).toElement().text();

			ui.searchList->addItem(cityName);
			searchResults.insert(i, cityId);
		}
	}
}

void weatherSettings::searchListRowChanged(int row)
{
	if (row != -1)
		ui.searchResult->setText(searchResults.at(row));
}

void weatherSettings::searchListItemDoubleClicked(QListWidgetItem *)
{
	if (ui.searchList->currentRow() != -1)
		ui.cityList->addItem(searchResults.at(ui.searchList->currentRow()));
}
