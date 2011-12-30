#include "rssfeed.h"


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
#include <QVariant>

namespace deliberate {



RssFeedPtrInt
RssFeed::Reinterpret (const RssFeed * p)
{
  return reinterpret_cast<RssFeedPtrInt>(p);
}

RssFeed*
RssFeed::Reinterpret (const QVariant v)
{
  RssFeed * f;
  f = reinterpret_cast<RssFeed*>(v.toULongLong());
  return f;
}

int RssFeed::FeedNum(1);

int RssFeed::type() { return 2000;}

RssFeed::~RssFeed ()
{
}

RssFeed::RssFeed()
:isSaved(false),
 isGood(false),
 isDir(false),
 isNewStory(false)
{
  FeedNum++;
}

RssFeed::RssFeed (const QString n)
:isSaved(false),
 isGood(false),
 isDir(false),
 isNewStory(false)
{
  nick = n;
}

RssFeed::RssFeed (const RssFeed &r)
: xmlLink(r.xmlLink),
 title(r.title),
 description(r.description),
 imgLink(r.imgLink),
 nick(r.nick),
 language(r.language),
 isSaved(r.isSaved),
 isGood(r.isGood),
 isDir(r.isDir),
 isNewStory(r.isNewStory)
{
}



RssFeed &
RssFeed::operator= (const RssFeed &other)
{
  xmlLink = other.xmlLink;
  webLink = other.webLink;
  title = other.title;
  description = other.description;
  imgLink = other.imgLink;
  nick = other.nick;
  language = other.language;
  isSaved = other.isSaved;
  isGood = other.isGood;
  isDir = other.isDir;
  isNewStory = other.isNewStory;
  return *this;
}

void
RssFeed::clear ()
{
   xmlLink="";
   webLink="";
   title="";
   description="";
   imgLink="";
   nick="";
   language="";
   isSaved=false;
   isGood=false;
}

void
RssFeed::Dump() const
{
#if DRSS_DEBUG
    qDebug() << " feed at XML   " << xmlLink;
    qDebug() << "         web   " << webLink;
    qDebug() << "         title " << title;
    qDebug() << "         descr " << description;
    qDebug() << "         Image " << imgLink;
    qDebug() << "         Nick  " << nick;
    qDebug() << "         isSaved " << isSaved;
    qDebug() << "         isGood  " << isGood;
    qDebug() << "         isDir   " << isDir;
#endif
}

}
