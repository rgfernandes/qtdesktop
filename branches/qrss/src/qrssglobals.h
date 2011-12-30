#ifndef RSSGLOBALS_H
#define RSSGLOBALS_H

#define DATEFORMAT "d MMM yyyy H:m:s"

#define CNFNAME "qrssconf.cnf"

#define ITEMSEPARATOR "__;__"
#define ITEMSEPARATORLENGTH 5

#define CHANNELSEPARATOR "__;;__"
#define CHANNELSEPARATORLENGTH 6;
#define RSSCNFEND "__;;;__"

#define DATEFORMAT "d MMM yyyy H:m:s"

enum ItemState
{
	eIdle = 0,
	eNew,
	eUnreaded,
	eReaded
};


//move to the different file
#ifdef WIN32

#define __LOG_FNAME__ __FUNCSIG__;

#else

#define __LOG_FNAME__ __PRETTY_FUNCTION__;

#endif

#endif //RSSGLOBALS_H
