#include "cmdoptions.h"
#include <iostream>

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

using namespace std;
namespace bpo = boost::program_options;

namespace deliberate {

CmdOptions::CmdOptions (string pgmname)
   :mPgm("")
{
  mPgm = pgmname;
  mDashOpts.add_options()
        ("help,h","give help message and exit")
        ("version,v","give version and exit")
	("feeds,f",bpo::value<string>(),"feed file name")
	("configfile,c",bpo::value<string>(),"configuration file name")
	;
}

bool
CmdOptions::Parse (int argc, char *argv[])
{
  try {
    bpo::store (bpo::command_line_parser (argc, argv).
                 options(mDashOpts).positional(mSimpleOpts).run(),
                 mOptValues);
    bpo::notify (mOptValues);
  } catch (...) {
    return false;
  }
  return true;  
}

void
CmdOptions::Usage ()
{
  cout << mPgm << ":" << endl;
  mDashOpts.print(cout);
}


bool
CmdOptions::SetStringOpt (const string name, string & opt)
{
  int seenOpt = mOptValues.count(name.c_str());
  if (seenOpt > 0) {
    opt = mOptValues[name.c_str()].as<string>();
    return true;
  }
  return false;
}

bool
CmdOptions::SetBoolOpt (const string name, bool & opt)
{
  int seenOpt = mOptValues.count(name.c_str());
  if (seenOpt > 0) {
    opt = true;
    return true;
  }
  return false;
}

bool 
CmdOptions::SeenOpt (const string name)
{
  return mOptValues.count(name.c_str()) > 0;
}

bool
CmdOptions::WantHelp ()
{
  return mOptValues.count("help") > 0;
}

bool
CmdOptions::WantVersion ()
{
  return mOptValues.count("version") > 0;
}

} // namespace
