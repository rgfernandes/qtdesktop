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
#include "docu.h"
#include "drssconfig.h"
#include "cmdoptions.h"
#include "version.h"
#include "drssdebug.h"

#include <QtGui/QApplication>
#include <QTimer>
#include <QString>
#include <iostream>

#include <QFile>

using namespace std;
using namespace deliberate;

int main (int argc, char *argv[])
{

  CmdOptions opt(MyName());
  DRSSConfig config;
  UseMyOwnMsgHandler ();
  QApplication App(argc, argv);
  bool cliOk = opt.Parse(argc, argv);
  
  if (!cliOk) {
    opt.Usage();
    exit(1);
  }
  if (opt.WantHelp()) {
    opt.Usage();
    exit(0);
  }
  
  if (opt.WantVersion()) {
    CLIVersion();
    exit(0);
  }
  
  /** @brief Configuration: 
    *    - first read default config file if it is there
    *    - second see if user said to read another one,
    *      read that one if it is there
    *    - third see what other options are on the command line,
    *      and set those
    */
    
  config.Read();
  QString tmp(config.ConfigFile());
  string configfile (tmp.toLocal8Bit().data());
  bool changedCF = opt.SetStringOpt ("configfile",configfile);
  config.SetConfigFile (configfile.c_str());
  if (changedCF) {
    config.Read();
  }
  
  tmp = config.FeedListFile();
  string feedfile (tmp.toLocal8Bit().data());
  opt.SetStringOpt ("feeds",feedfile);
  config.SetFeedListFile (feedfile.c_str());
  
  Docu theDoc(&App, config);
    
  theDoc.Show();
  App.exec();
  
}
