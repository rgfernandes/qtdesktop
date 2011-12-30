#include "editfeed.h"

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

#include "drssdebug.h"

namespace deliberate {

EditFeed::EditFeed ()
{
  setupUi(this);
  mProbeReply = 0;
  ConnectButtons ();
  hide();
  loadingPic.load(":/loading.png");
  notloadingPic.load(":/notloading.png");
}

void
EditFeed::ConnectButtons ()
{
  connect (cancelButton,SIGNAL(clicked()), this, SLOT(Cancel()));
  connect (deleteButton, SIGNAL(clicked()), this, SLOT(Delete()));
  connect (doneButton, SIGNAL(clicked()), this, SLOT(Done()));
}

void
EditFeed::DisableButtons ()
{
  deleteButton->setEnabled(false);
  doneButton->setEnabled(false);
}

void
EditFeed::EnableButtons ()
{
  cancelButton->setEnabled(true);
  deleteButton->setEnabled(true);
  if (isgood) {
    doneButton->setEnabled(true);
  }
}


void
EditFeed::Save ()
{
  if (pFeed) {
    pFeed->SetSaved(true);
    Done(false);
  }
}

void
EditFeed::Delete ()
{
  if (pFeed) {
    pFeed->SetSaved(false);
    Done(true);
  }
}

void
EditFeed::Edit (QStandardItem *pItem)
{
   if (pItem == 0) {
      return;
   }
   pFeed = RssFeed::Reinterpret(pItem->data());
   pReturnItem = pItem;
   isgood = true;
   descriptionLineEdit->setPlainText (pFeed->Descr());
   nickLineEdit->setText (pFeed->Nick());
   titleLineEdit->setText (pFeed->Title());
   websiteLineEdit->setText (pFeed->WebLink());
   xmlLinkLineEdit->setText (pFeed->XmlLink());
   if (pFeed->Dir()) {
     loadStatus->setText(tr("Folder"));
     
   } else {
     loadStatus->setText("");
   }
   
   show();
}

void
EditFeed::EditNew (QNetworkAccessManager * pQMgr,
                    const QString newxml, QStandardItem *pItem)
{
  if (pItem == 0) {
    return;
  }
  pFeed = RssFeed::Reinterpret(pItem->data());
  pReturnItem = pItem;
  if (pFeed == 0) {
    return;
  }
  isgood = false;
  xmlLinkLineEdit->setText(newxml);
  descriptionLineEdit->setText("");
  nickLineEdit->setText("");
  titleLineEdit->setText("");
  websiteLineEdit->setText("");
  ProbeChannel (pQMgr, newxml,pFeed);
  DisableButtons();
  show();
}

void
EditFeed::Done (bool erase)
{
  RssFeed *pRF = pFeed;
  pFeed = 0;
  hide();
  pRF->SetDescr (descriptionLineEdit->toPlainText());
  pRF->SetNick (nickLineEdit->text());
  pRF->SetTitle (titleLineEdit->text());
  pRF->SetWebLink (websiteLineEdit->text());
  pRF->SetXmlLink (xmlLinkLineEdit->text());
  pRF->SetSaved(!erase);
  emit FinishedEditing (pReturnItem, true, false, erase);
}


void
EditFeed::Cancel ()
{
  pFeed = 0;
  hide();
  emit FinishedEditing (pReturnItem, false, false, false);
}

void
EditFeed::ProbeChannel (QNetworkAccessManager * pQMgr,
                        const QString link, RssFeed *pRF)
{
  if (mProbeReply) {
    return;
  }
  newFeed = pRF;
  pRF->SetSaved(true);
  mProbeReply = pQMgr->get(QNetworkRequest(QUrl(link)));
  connect (mProbeReply, SIGNAL(finished()), this, SLOT(CatchProbe()));
  loadStatus->setPixmap(loadingPic);
  isgood = false;
}

void
EditFeed::PopulateFromRssDoc (QDomElement & el)
{
  static QString tag_title("title");
  static QString tag_link("link");
  static QString tag_description("description");
  QString t;
  QString weblink;
  QString title;
  QString description (tr("cannot find RSS feed data"));
  bool foundsomething(false);
  for (QDomElement child = el.firstChildElement();
       !child.isNull();
       child = child.nextSiblingElement()) {
    t = child.tagName();
    if (t == tag_title) {
      title = child.text();
      foundsomething = true;
    } else if (t == tag_link) {
      weblink = child.text();
      foundsomething = true;
    } else if (t == tag_description) {
      description = child.text();
    }
  }
  titleLineEdit->setText(title);
  websiteLineEdit->setText(weblink);
  descriptionLineEdit->setText(description);
  isgood = foundsomething;
  show();
}


void
EditFeed::PopulateFromAtomDoc (QDomElement & el)
{
  QString t;
  static QString tag_title("title");
  static QString tag_link("link");
  static QString tag_author("author");
  static QString tag_subtitle("subtitle");
  QString xmllink;
  QString weblink;
  QString title;
  QString author;
  QString description (tr("cannot find Atom feed data"));
  bool foundsomething(false);
  for (QDomElement child = el.firstChildElement();
       !child.isNull();
       child = child.nextSiblingElement()) {
    t = child.tagName();
    if (t == tag_title) {
      title = child.text();
      foundsomething = true;
    } else if (t == tag_link) {
      foundsomething |= ParseAtomLinkElem (child, xmllink, weblink);
    } else if (t == tag_author) {
      foundsomething |= ParseAtomAuthorElem (child, author);
    } else if (t == tag_subtitle) {
      description = child.text();
    }
  }
  titleLineEdit->setText(title);
  websiteLineEdit->setText(weblink);
  descriptionLineEdit->setText(description);
  isgood = foundsomething;
  show();
}

bool 
EditFeed::ParseAtomLinkElem (QDomElement & el, QString & xml, QString & web)
{
   QString relAttr = el.attribute("rel");
   bool ok(false);
   if (relAttr == "self") {
     xml = el.attribute("href");
     ok = true;
   } else {
     web = el.attribute("href");
     ok = true;
   }
   return ok;
}

bool
EditFeed::ParseAtomAuthorElem (QDomElement &el, QString & name)
{
  bool ok(false);
  for (QDomElement child = el.firstChildElement();
       !child.isNull();
       child = child.nextSiblingElement()) {
    if (child.tagName() == "name") {
      name = child.text();
      ok = name.length() > 0;
    }
  }
  return ok;
}



void
EditFeed::CatchProbe()
{
  QNetworkReply *reply = mProbeReply;
  disconnect (reply,SIGNAL(finished()), this, SLOT(CatchProbe()));
  QDomDocument doc;
  bool ok = doc.setContent (reply);
  QDomElement root = doc.documentElement();
  if (!ok) {
    descriptionLineEdit->setText(tr("cannot find Feed data"));
     // do something like fill in the form with disclaimers
  } else {
    if (root.tagName() == "rss" || root.tagName() == "rdf:RDF") {  // RSS
      for (QDomElement rssel = root.firstChildElement();
           !rssel.isNull();
           rssel = rssel.nextSiblingElement()) {
        if (rssel.tagName() == "channel") {
          PopulateFromRssDoc (rssel);
        } 
      }
    } else if (root.tagName() == "feed") { // Atom
      PopulateFromAtomDoc (root);
    } else {  
      descriptionLineEdit->setText(tr("cannot decode Feed data"));
    }
  }
  mProbeReply = 0;
  loadStatus->setPixmap(notloadingPic);
  EnableButtons();
}

}
