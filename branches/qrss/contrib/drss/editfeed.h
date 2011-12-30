#ifndef EDITFEED_H
#define EDITFEED_H

//
//  Copyright (C) 2009 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
#include <QDialog>
#include <QDomElement>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QStandardItem>
#include "ui_feededit.h"
#include "rssfeed.h"

namespace deliberate {

class EditFeed : public QDialog, public Ui_FeedEdit {

Q_OBJECT

public:
 
   EditFeed ();
   
   void Edit (QStandardItem *pItem);
   void EditNew (QNetworkAccessManager * pQMgr,
                 const QString newxml, QStandardItem *pItem);
   
public slots:

  void Cancel();
  void Save();
  void Delete();
  void Done(bool erase = false);
  void CatchProbe ();
   
signals:

  void FinishedEditing (QStandardItem *f, bool changed, bool checkit, bool erase);
  
private:

  void ConnectButtons();
  void DisableButtons();
  void EnableButtons();
  void ProbeChannel (QNetworkAccessManager * pQMgr,
              const QString link, RssFeed *pRF);
  void PopulateFromRssDoc (QDomElement & el);
  void PopulateFromAtomDoc (QDomElement & el);
  bool ParseAtomLinkElem   (QDomElement & el, QString & xml, QString & web);
  bool ParseAtomAuthorElem (QDomElement & el, QString & name);
  
  QStandardItem * pReturnItem;
  RssFeed   * pFeed;
  RssFeed   * newFeed;
  QNetworkReply         * mProbeReply;
  bool                 isgood;
  
  
  QPixmap              loadingPic;
  QPixmap              notloadingPic;

};

}

#endif
