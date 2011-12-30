#ifndef FEEDLIST_H
#define FEEDLIST_H


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

#include <map>
#include <QStandardItemModel>
#include <QString>
#include <QDomDocument>
#include "rssfeed.h"

using namespace std;

namespace deliberate {

class FeedList : public QStandardItemModel   {

Q_OBJECT

public:

   enum FeedTag { t_none, 
                  t_drssFeedList,
                  t_feed,
                  t_language,
                  t_xmlLink, 
                  t_webLink,
                  t_title, 
                  t_descr,
                  t_imgLink,
                  t_nick,
                  t_dir,
                  t_Last
                  };


  FeedList();
  ~FeedList();
  
  void Init();
  void Load (QIODevice *dev);
  void LoadOpml (QIODevice *dev);
  void Save (QIODevice *dev);
  void clear();
  
  void CheckChanged (bool clear=false);
  void SlowCheckChanged (QStandardItem * pItem);
  bool RecurseCheckChanged (QStandardItem *pParent, 
                            bool doEmit = false, 
                            bool clear = false);
  
  void Insert (RssFeed *f, QStandardItem * pItem = 0);
  void InsertNewDir (RssFeed *f);
  
  bool Delete (QStandardItem *f);
  
  QModelIndex indexFromItem (const QStandardItem *feed) 
     { return QStandardItemModel::indexFromItem (feed);}
  QStandardItem * itemFromIndex (const QModelIndex & ndx) 
     { return QStandardItemModel::itemFromIndex (ndx); }
     
  QModelIndex RootIndex () 
     { return indexFromItem (rootNode); }
    
  bool      Empty();
  
public slots:

signals:

  void QueryWeb (RssFeed *);

private:

  void Domify();
  void DomifyRecurseDir (QDomElement & dir, QStandardItem * pParent);
  void FillInDom (QDomElement & el, RssFeed & rf);
  void AppendTextElement (QDomElement &el, QString tag, QString text);
  
  void     ParseFeed (QDomElement &feedEl);
  void     RecurseDrssDir (QDomElement &el);
  void     LoadOpmlFeed (QDomElement &el);
  void     RecurseOpml  (QDomElement &el);
  void     PushNewDir   (QDomElement &el);
  void     PopDir       ();
  
  void     MakeBold (QStandardItem *pItem, bool bold = true);
  void     SetDirDeco (QStandardItem *pItem, bool isDir = true);
  
  FeedTag  MatchTag (QString t);
  
  QDomDocument     dom;
  
  QStandardItem   * rootNode;
  QStandardItem   * buildRoot;
  
  #if 0
  typedef map<int,RssFeed*> FeedMapType;
  
  FeedMapType feedMap;
  
  static int NextFeedId;
  
  static int NewItemId() { return NextFeedId++; }
  #endif
  
  QString TagString[t_Last];
  
};
}

#endif
