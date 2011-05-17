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
#include <QApplication>         // to use qApp->font()
#include <QFont>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QtDebug> 		//to use qWarning and qDebug messages

#include "config.h"
#include "profile.h"
#include "docuparser.h"
#include "pcommon.h"


static Config *static_configuration = 0;

//-------------------------------------------------
Config::Config()
	: profil(0), hideSidebar(false), rebuildDocs(true)
{
	if( !static_configuration ) {
		static_configuration = this;
	} else {
		qWarning( "Multiple configurations not allowed!" );
	}
}

//-------------------------------------------------
Config *Config::configuration()
{
	Q_ASSERT( static_configuration );
	return static_configuration;
}

//-------------------------------------------------
void Config::loadProject(const QString &projectFileName)
{
	toAppLog(2, "Open project: " + projectFileName);
	QString prjFN = projectFileName;
	QFileInfo fi(prjFN);

	if ( !(fi.exists() && fi.isFile()) ){
		qWarning( (QLatin1String("Project does not exist: ") + prjFN).toAscii().constData() );
		toAppLog(2, "- project does not exist: " + prjFN);
		prjFN = PrjDir() + "rahelp/rahelp.rap";
		fi.setFile(prjFN);
		qWarning( (QLatin1String("Open RA help project instead: ") + prjFN).toAscii().constData() );
		toAppLog(2, "- open RA help project instead: " + prjFN);
	}

	setCurProject(prjFN);
	setCurPrjDir(fi.absolutePath());
	DocuParser *parser = DocuParser::createParser( prjFN );
	if (!parser) {
		qWarning( (QLatin1String("Failed to create parser for file: ") + prjFN).toAscii().constData() );
		toAppLog(2, "- failed to create parser for file: " + prjFN);
	}
	DocuParserRAP *profileParser = static_cast<DocuParserRAP*>(parser);
	QFile file(prjFN);
	parser->parse(&file);
	profil = profileParser->profile();
	if (!profil) {
		qWarning( (QLatin1String("Config::loadProject(), no profile in: ") + prjFN).toAscii().constData() );
		toAppLog(2, "- no profile in: " + prjFN);
	}
	profil->setDocuParser(profileParser);
	setCurFile(profil->props["startpage"]);
	setCurPrjName(profil->props["title"]);
	setCurPrjSrc();
	prjLogFN = AppDir() + "log/" + profileName() + ".log";
    qDebug() << " curProject = " << curProject <<", curFile = " << curFile;
	toAppLog(2, "- loaded succesfully");
}

//-------------------------------------------------
void Config::loadSettings()
{
	QSettings settings(iniFile, QSettings::IniFormat);
	setAppLogLevel( settings.value(QLatin1String("LogLevel-Application") ).toInt() );
	setPrjLogLevel( settings.value(QLatin1String("LogLevel-Project") ).toInt() );
	toAppLog(2, "Load application settings");

	//miscellaneous settings
	lang = settings.value(QLatin1String("Language") ).toString();
	src = settings.value(QLatin1String("Source") ).toStringList();
	QStringList::iterator it = src.begin();
	for (; it != src.end(); ++it) {
		*it = urlifyFileName( absolutifyFileName(*it, prjDir) );
	}
	sideBar = settings.value(QLatin1String("SideBarPage") ).toInt();
	rebuildDocs = settings.value(QLatin1String("RebuildDocDB"), true ).toBool();
	profileFNs = settings.value(QLatin1String("Projects") ).toStringList();
	profileFNs = absolutifyFileList(profileFNs, prjDir);	//we will load this list to widget in HelpDialog::initialize()
	curProject = settings.value(QLatin1String("ActiveProject") ).toString();
	curProject = absolutifyFileName(curProject, prjDir);	 //absolutify project file path
	setContentsSortOrder(settings.value(QLatin1String("ContentsSortOrder") ).toString());

	//window and font settings
	winGeometry = settings.value(QLatin1String("WindowGeometry")).toByteArray();
	mainWinState = settings.value(QLatin1String("MainWindowState")).toByteArray();
	pointFntSize = settings.value(QLatin1String("FontSize"), qApp->font().pointSizeF()).toDouble();
	m_fontSettings.windowFont = qVariantValue<QFont>(settings.value(QLatin1String("WindowFont"), qApp->font()));
	m_fontSettings.browserFont = qVariantValue<QFont>(settings.value(QLatin1String("BrowserFont"), qApp->font()));
	m_fontSettings.useWindowFont = settings.value(QLatin1String("UseWindowFont"), false).toBool();
	m_fontSettings.useBrowserFont = settings.value(QLatin1String("UseBrowserFont"), false).toBool();
	m_fontSettings.windowWritingSystem = static_cast<QFontDatabase::WritingSystem>(
		settings.value(QLatin1String("WindowWritingSystem"), QFontDatabase::Latin).toInt());
	m_fontSettings.browserWritingSystem = static_cast<QFontDatabase::WritingSystem>(
		settings.value(QLatin1String("BrowserWritingSystem"), QFontDatabase::Latin).toInt());
	m_fontSettings.browserFont.setPointSizeF(pointFntSize);

	//settings from Settings window
	setContentsAdditionalView(	settings.value(QLatin1String("ContentsAdditionalView")).toBool() );
	setShowSubItemsTitle(		settings.value(QLatin1String("ShowSubItemsTitle")).toBool() );
	setAutoCollapse(			settings.value(QLatin1String("AutoCollapse")).toBool() );
	backupDir = settings.value(QLatin1String("BackupPath") ).toString();
	backupDir = absolutifyFileName(backupDir, prjDir);		//absolutify path to backup dir
	externalEditor = settings.value(QLatin1String("ExternalEditor") ).toString();
	externalEditor = absolutifyFileName(externalEditor, prjDir); //absolutify file path to editor
	externalBrowser = settings.value(QLatin1String("ExternalBrowser") ).toString();
	externalBrowser = absolutifyFileName(externalBrowser, prjDir);	//absolutify file path to browser
	externalArchiver = settings.value(QLatin1String("ExternalArchiver") ).toString();
	externalArchiver = absolutifyFileName(externalArchiver, prjDir);	//absolutify file path to archiver
	externalArchiverOptions = settings.value(QLatin1String("ExternalArchiverOptions") ).toString();
	setItemAutoProperties(settings.value(QLatin1String("ItemAutoProperties")).toBool() );
	setAcceptDropImages(settings.value(QLatin1String("AcceptDropImages")).toBool() );

	//settings from Signature window
	setDefaultSignatureID(		settings.value(QLatin1String("DefaultSignatureID")).toInt() );

	toAppLog(2, "- done");
}//loadSettings()

//-------------------------------------------------
void Config::saveSettings()
{
	toAppLog(2, "Save application settings");
	QSettings settings(iniFile, QSettings::IniFormat);

	//miscellaneous settings
	settings.setValue(QLatin1String("Language"), lang);
	settings.setValue(QLatin1String("Projects"), relatifyFileList(profileFNs, prjDir) );
	settings.setValue(QLatin1String("ActiveProject"), relatifyFileName(curProject, prjDir) );
	settings.setValue(QLatin1String("Source"),src );	 //paths relatified in MainWindow::saveSettings()
	settings.setValue(QLatin1String("SideBarPage"), sideBarPage() );
	settings.setValue(QLatin1String("RebuildDocDB"), rebuildDocs );
	settings.setValue(QLatin1String("ContentsSortOrder"), contentsSortOrder );

	//window and font settings
	settings.setValue(QLatin1String("WindowGeometry"), winGeometry);
	settings.setValue(QLatin1String("MainWindowState"), mainWinState );
	settings.setValue(QLatin1String("FontSize"), pointFntSize);
	settings.setValue(QLatin1String("WindowFont"), m_fontSettings.windowFont);
	settings.setValue(QLatin1String("BrowserFont"), m_fontSettings.browserFont);
	settings.setValue(QLatin1String("UseWindowFont"), m_fontSettings.useWindowFont);
	settings.setValue(QLatin1String("UseBrowserFont"), m_fontSettings.useBrowserFont);
	settings.setValue(QLatin1String("WindowWritingSystem"), m_fontSettings.windowWritingSystem);
	settings.setValue(QLatin1String("BrowserWritingSystem"), m_fontSettings.browserWritingSystem);

	//settings from Settings window
	settings.setValue(QLatin1String("ExternalEditor"), relatifyFileName(externalEditor, prjDir) );
	settings.setValue(QLatin1String("ExternalBrowser"), relatifyFileName(externalBrowser, prjDir) );
	settings.setValue(QLatin1String("ExternalArchiver"), relatifyFileName(externalArchiver, prjDir) );
	settings.setValue(QLatin1String("ExternalArchiverOptions"), ExternalArchiverOptions());
	settings.setValue(QLatin1String("BackupPath"), relatifyFileName(backupDir, prjDir) );
	settings.setValue(QLatin1String("ContentsAdditionalView"), contentsAdditionalView);
	settings.setValue(QLatin1String("ShowSubItemsTitle"), showSubItemsTitle);
	settings.setValue(QLatin1String("AutoCollapse"),autoCollapse);
	settings.setValue(QLatin1String("LogLevel-Application"),AppLogLevel());
	settings.setValue(QLatin1String("LogLevel-Project"),PrjLogLevel());
	settings.setValue(QLatin1String("ItemAutoProperties"),ItemAutoProperties());
	settings.setValue(QLatin1String("AcceptDropImages"),AcceptDropImages());

	//settings from Signature window
	settings.setValue(QLatin1String("DefaultSignatureID"),DefaultSignatureID());

	toAppLog(2, "- done");
}//saveSettings()

//-------------------------------------------------
QStringList Config::mimePaths() //where to make full search. Used only in TabbedBrowser::createHelpWindow
{
	static QStringList lst;

	if( lst.count() > 0 )
		return lst;

	lst << CurPrjDir();
	return lst;
}

//-------------------------------------------------
void Config::delProject(QString prj)
{
	int i, index=-1;
	int n = profileFNs.count();
	for (i=0; i<n; i++)
		if (profileFNs[i] == prj)
			index = i;
	if (index >=0)
		profileFNs.removeAt(index);
}

//-------------------------------------------------
QString Config::profileTitle() const
{
	QString s = profil->props[QLatin1String("title")];
	if (s.isEmpty())
		s = QObject::tr("[No title]");
	return s;
}

//-------------------------------------------------
QString Config::homePage() const
{
	return startPage.isEmpty() ? profil->props[QLatin1String("startpage")] : startPage;
}

//-------------------------------------------------
QStringList Config::source() const
{
	return src.size() == 0 ? QStringList(profil->props[QLatin1String("startpage")]) : src;
}

//-------------------------------------------------
QStringList Config::docFiles() const //used in HelpDialog to build keyword DB. Earlier it used to return profil->docs
{
	QStringList tmp;
	tmp << curProject; //probably will need to change to something else.
	return tmp;
}

//-------------------------------------------------
void Config::hideSideBar( bool b ) {  hideSidebar = b; }

//-------------------------------------------------
bool Config::sideBarHidden() const {  return hideSidebar; }

//-------------------------------------------------
QString Config::getProjectProperty(QString prop, QString prjFN)
{
	QFile file(prjFN);
	if (!file.exists()) {
		toAppLog(1, QString("Project file does not exist: %1").arg(prjFN));
		return "";
	}
	DocuParser *parser = DocuParser::createParser( prjFN );
	if (!parser) {
		toAppLog(1, QString("Failed to create parser for project: %1").arg(prjFN));
		return "";
	}
	DocuParserRAP *profileParser = static_cast<DocuParserRAP*>(parser);
	parser->parse(&file);
	profil_tmp = profileParser->profile();
	if (!profil_tmp) {
		toAppLog(1, QString("No profile in: %1").arg(prjFN));
		return "";
	}
	return profil_tmp->props[prop];
}

//-------------------------------------------------
void Config::setCurPrjSrc()
{
	curPrjSrc = CurProject();
	curPrjSrc.chop(4);
	dbName = curPrjSrc + "-sources.db";
	curPrjSrc = curPrjSrc + "-sources.xml";	// !+! for future XML import-export
}

//-------------------------------------------------
void Config::setFontPointSize(qreal size)
{
	pointFntSize = size;
	m_fontSettings.useBrowserFont = true;
	m_fontSettings.browserFont.setPointSizeF(size);
}

//-------------------------------------------------
void Config::toAppLog(int logLevel, QString msg)
{
	if (logLevel <= AppLogLevel())
		toLog(AppLogFN() ,msg);
}

//-------------------------------------------------
void Config::toPrjLog(int logLevel, QString msg)
{
	if (logLevel <= PrjLogLevel())
		toLog(PrjLogFN() ,msg);
}
