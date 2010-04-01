/*
	ClientConfigPage

	Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef CLIENTCONFIGPAGE_H
#define CLIENTCONFIGPAGE_H

#include <QtGui/QWizardPage>
#include "historymanagerwindow.h"

namespace Ui {
    class ClientConfigPage;
}

namespace HistoryManager {

class ClientConfigPage : public QWizardPage {
    Q_OBJECT
    Q_DISABLE_COPY(ClientConfigPage)
public:
	explicit ClientConfigPage(HistoryManagerWindow *parent = 0);
    virtual ~ClientConfigPage();
	static QString getAppropriatePath(const QString &path);
	static QString getAppropriateFilePath(const QString &filename);

protected:
    virtual void changeEvent(QEvent *e);
	virtual void initializePage();
	virtual void cleanupPage();
	virtual bool validatePage();
	virtual bool isComplete() const;
	virtual int nextId() const;

protected slots:
	void on_browseButton_clicked();
	void onTextChanged(const QString &filename);

private:
    Ui::ClientConfigPage *m_ui;
	bool m_valid;
	HistoryManagerWindow *m_parent;
	QPixmap m_valid_pixmap;
	QPixmap m_invalid_pixmap;
	QList<ConfigWidget> m_config_list;
};

}

#endif // CLIENTCONFIGPAGE_H
