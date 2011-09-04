/****************************************************************************
**
** Copyright (C) 2007-2009 Pavel Malakhov
**
** This file is part of Research Assistant project
** https://sourceforge.net/projects/rassistant
**
** The code is under GNU General Public License versions 2.0 or 3.0
** It is based on Trolltech Qt Assistant 4.3, http://trolltech.com
****************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include "profile.h"
#include "docuparser.h"

#include <QString>
#include <QStringList>
//#include <QMap>

#include <QtGui/QFont>
#include <QtGui/QFontDatabase>

#define RA_VERSION_STR	"0.8.2"
#define RA_BUILD_STR	"2009.07.16"

class Profile;

struct FontSettings
{
	FontSettings() : useWindowFont(false), useBrowserFont(false),
		windowWritingSystem(QFontDatabase::Latin), browserWritingSystem(QFontDatabase::Latin)
		{ }

	QFont windowFont;
	QFont browserFont;

	bool useWindowFont;
	bool useBrowserFont;

	QFontDatabase::WritingSystem windowWritingSystem;
	QFontDatabase::WritingSystem browserWritingSystem;
};

//====================== class Config ============================
class Config
{
public:

	Config();

	void loadSettings();
	void saveSettings();
	Profile *profile() const { return profil; }
	QString profileName() const { return profil->props[QLatin1String("name")]; }
	QString profileTitle() const;
	bool validProfileName() const;
	void hideSideBar( bool b );
	bool sideBarHidden() const;
	QStringList mimePaths();
	QString getProjectProperty(QString prop, QString prjFN); //especially for not current projects. Gets projects' property without loading.
	void toAppLog(int logLevel, QString msg);
	void toPrjLog(int logLevel, QString msg);


	// From profile, read only
	QStringList docFiles() const;
	QStringList ProfileFNs() const { return profileFNs; }
	void addProject(QString prj) { profileFNs << prj; }
	void delProject(QString prj);

	// From QSettings, read / write
	QString homePage() const;
	QStringList source() const;

	void setHomePage( const QString &hom ) { startPage = hom; }
	void setSource( const QStringList &s ) { src = s; }

	int sideBarPage() const { return sideBar; }
	void setSideBarPage( int sbp ) { sideBar = sbp; }

	QByteArray windowGeometry() const { return winGeometry; }
	void setWindowGeometry( const QByteArray &geometry ) { winGeometry = geometry; }

	QByteArray mainWindowState() const { return mainWinState; }
	void setMainWindowState( const QByteArray &state ) { mainWinState = state; }

	qreal fontPointSize() const 						{ return pointFntSize; }
	void setFontPointSize(qreal size);
	FontSettings fontSettings() 						{ return m_fontSettings; }
	void setFontSettings(const FontSettings &settings)	{ m_fontSettings = settings; }

	bool docRebuild() const			{ return rebuildDocs; }
	void setDocRebuild( bool rb )	{ rebuildDocs = rb; }

	static Config *configuration();
	void loadProject(const QString &projectFileName);

	//variables for global use via  Config::configuration()->variable()
	QString AppDir()	{ return appDir; }
	QString BackupDir()	{ return backupDir; }
	QString CacheDir()	{ return cacheDir; }
	QString PrjDir()	{ return prjDir; }
	QString ImgDir()	{ return imgDir; }
	QString CurPrjDir()	{ return curPrjDir; }
	QString CurPrjImgDir()	{ return curPrjImgDir; }
	QString CurProject(){ return curProject; }
	QString CurFile()	{ return curFile; }
	QString CurPrjSrc()	{ return curPrjSrc; }
	QString CurPrjName(){ return curPrjName; }
	QString Lang()		{ return lang; }
	QString ErrPage()	{ return errPage; }
	QString IniFile()	{ return iniFile; }
	QString DbName()	{ return dbName; }
	QString ContentsSortOrder()	{ return contentsSortOrder; }
	QString IndentString()	{ return "   "; }

	void setAppDir(QString dir)		{ appDir = dir; }
	void setBackupDir(QString dir)	{ backupDir = dir; }
	void setCacheDir(QString dir)	{ cacheDir = dir; }
	void setPrjDir(QString dir)		{ prjDir = dir; }
	void setImgDir(QString dir)		{ imgDir = dir; }
	void setCurPrjDir(QString dir)	{ curPrjDir = dir; curPrjImgDir = dir + "/images";}
	//void setCurPrjImgDir(QString dir)	{ curPrjImgDir = dir; }
	void setCurProject(QString prjFN)	{ curProject = prjFN; }
	void setCurFile(QString fn)		{ curFile = fn; }
	void setCurPrjSrc();
	void setCurPrjName(QString name){ curPrjName = name; }
	void setLang(QString lng)		{ lang = lng; }
	void setErrPage(QString fn)		{ errPage = fn; }
	void setIniFile(QString fn)		{ iniFile = fn; }
	void setDbName(QString fn)		{ dbName = fn; }
	void setContentsSortOrder(QString order)	{ contentsSortOrder = order; }

	//variables from Settings window
	QString AppLogFN()	{ return appLogFN; }
	QString PrjLogFN()	{ return prjLogFN; }
	int AppLogLevel()	{ return appLogLevel; }
	int PrjLogLevel()	{ return prjLogLevel; }
	QString ExternalEditor()	{ return externalEditor; }
	QString ExternalBrowser()	{ return externalBrowser; }
	QString ExternalArchiver()	{ return externalArchiver; }
	QString ExternalArchiverOptions()	{ return externalArchiverOptions; }
	bool ContentsAdditionalView()	{ return contentsAdditionalView; }
	bool ShowSubItemsTitle()		{ return showSubItemsTitle; }
	bool AutoCollapse()				{ return autoCollapse; }
	bool ItemAutoProperties()		{ return itemAutoProperties; }
	bool AcceptDropImages()			{ return acceptDropImages; }

	void setAppLogFN(QString fn)	{ appLogFN = fn; }
	void setAppLogLevel(int n)		{ appLogLevel = n; }
	void setPrjLogLevel(int n)		{ prjLogLevel = n; }
	void setItemAutoProperties(bool b)	{ itemAutoProperties = b; }
	void setAcceptDropImages(bool b)	{ acceptDropImages = b; }
	void setExternalEditor(QString fn)	{ externalEditor = fn; }
	void setExternalBrowser(QString fn)	{ externalBrowser = fn; }
	void setExternalArchiver(QString fn){ externalArchiver = fn; }
	void setExternalArchiverOptions(QString opts)	{ externalArchiverOptions = opts; }

	//variables from Signature window
	int DefaultSignatureID()	{ return defaultSignatureID; }

	void setDefaultSignatureID(int id)	{ defaultSignatureID = id; }

public slots:
	void setContentsAdditionalView(bool checked)	{ contentsAdditionalView = checked; }
	void setShowSubItemsTitle(bool checked)			{ showSubItemsTitle = checked; }
	void setAutoCollapse(bool checked)				{ autoCollapse = checked; }

private:
	Config( const Config &c );
	Config& operator=( const Config &c );

private:
	Profile *profil;
	Profile *profil_tmp; //to build project list

	QStringList profileFNs;	//list of projects
	QString startPage;  //show this page when application is started.  Should be last veiwed page before app close
	QStringList src;    //opened files in tab browser
	QByteArray mainWinState;
	QByteArray winGeometry;
	qreal pointFntSize;
	int sideBar;
	bool hideSidebar;
	bool rebuildDocs;
	FontSettings m_fontSettings;

	//variables for global use via  Config::configuration()->variable()
	QString appDir;
	QString backupDir;
	QString cacheDir;	//path for cache
	QString prjDir;
	QString imgDir;
	QString curPrjDir;
	QString curPrjImgDir;
	QString curProject;	// current project file name *.rap
	QString curFile;	// opened file. We need this to save any changes automaticly
	QString curPrjSrc;
	QString curPrjName;
	QString lang;
	QString errPage;	// html file to output application errors
	QString iniFile;
	QString appLogFN;
	QString prjLogFN;
	int appLogLevel;
	int prjLogLevel;
	QString dbName;
	QString contentsSortOrder;
		//variables from Settings window
	bool contentsAdditionalView;
	bool showSubItemsTitle;
	bool autoCollapse;
	bool itemAutoProperties;
	bool acceptDropImages;
	QString externalEditor;
	QString externalBrowser;
	QString externalArchiver;
	QString externalArchiverOptions;
	int defaultSignatureID;
};

#endif // CONFIG_H
