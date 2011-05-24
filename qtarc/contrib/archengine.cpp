#include "archengine.h"

ArchEngine::ArchEngine( const QString & fn )
	: QFSFileEngine( fn ) {
	setFileName( "ZIP FILE" );
}

ArchEngine *ArchEngineHandler::create(const QString &fileName) const
{
	return new ArchEngine(fileName);
}
