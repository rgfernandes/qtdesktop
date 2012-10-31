#include <QtCore>
#include <QtGui>

#include <string>
#include <iostream>

#include <stdlib.h>

// TODO: QLocale (QLocale::system().name())

typedef struct {
	QLibraryInfo::LibraryLocation e;
	std::string s;
} QLI;

typedef struct {
	QDesktopServices::StandardLocation e;
	std::string s;
} QDS;

static QLI qli[] = {
	{ QLibraryInfo::PrefixPath, "QLibraryInfo::PrefixPath"},
	{ QLibraryInfo::DocumentationPath, "QLibraryInfo::DocumentationPath"},
	{ QLibraryInfo::HeadersPath, "QLibraryInfo::HeadersPath"},
	{ QLibraryInfo::LibrariesPath, "QLibraryInfo::LibrariesPath"},
	{ QLibraryInfo::BinariesPath, "QLibraryInfo::BinariesPath"},
	{ QLibraryInfo::PluginsPath, "QLibraryInfo::PluginsPath"},
	{ QLibraryInfo::DataPath, "QLibraryInfo::DataPath"},
	{ QLibraryInfo::TranslationsPath, "QLibraryInfo::TranslationsPath"},
	{ QLibraryInfo::SettingsPath, "QLibraryInfo::SettingsPath"},
	{ QLibraryInfo::ExamplesPath, "QLibraryInfo::ExamplesPath"},
	{ QLibraryInfo::DemosPath, "QLibraryInfo::DemosPath"}
};

static QDS qds[] {
	{ QDesktopServices::DesktopLocation, "QDesktopServices::DesktopLocation"},
	{ QDesktopServices::DocumentsLocation, "QDesktopServices::DocumentsLocation"},
	{ QDesktopServices::FontsLocation, "QDesktopServices::FontsLocation"},
	{ QDesktopServices::ApplicationsLocation, "QDesktopServices::ApplicationsLocation"},
	{ QDesktopServices::MusicLocation, "QDesktopServices::MusicLocation"},
	{ QDesktopServices::MoviesLocation, "QDesktopServices::MoviesLocation"},
	{ QDesktopServices::PicturesLocation, "QDesktopServices::PicturesLocation"},
	{ QDesktopServices::TempLocation, "QDesktopServices::TempLocation"},
	{ QDesktopServices::HomeLocation, "QDesktopServices::HomeLocation"},
	{ QDesktopServices::DataLocation, "QDesktopServices::DataLocation"},
	{ QDesktopServices::CacheLocation, "QDesktopServices::CacheLocation"}
};

void showall() {
	// 1. QLibraryInfo
	for (unsigned int i = 0; i < 11; i++)
		std::cout << qli[i].s << ": " << QLibraryInfo::location(qli[i].e).toStdString() << std::endl;
	// 2. QDesktopServices
	for (unsigned int i = 0; i < 11; i++)
		std::cout << qds[i].s << ": " << QDesktopServices::storageLocation(qds[i].e).toStdString() << std::endl;
	//QDesktopServices::displayName()
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	// 1. show std values
	showall();
	// 2. set app
        QCoreApplication::setApplicationName("QtInfo");
        QCoreApplication::setApplicationVersion("0.0.1");
        QCoreApplication::setOrganizationDomain("QtDesktop");
        QCoreApplication::setOrganizationName("TI_Eugene");
	// 3. show app values
	showall();
	return 0;
}
