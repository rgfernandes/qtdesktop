PROJECT( QRSS ) 
cmake_minimum_required (VERSION 2.6)
find_package(Qt4 REQUIRED )
if(WIN32)
ADD_DEFINITIONS( -Wall )
else(WIN32)
ADD_DEFINITIONS( -Wall -O2 )
endif(WIN32)
set(QT_USE_QTNETWORK 1)
set(QT_USE_QTXML 1)
set(QT_USE_QTWEBKIT 1)	


SET( SRCS 
	src/main.cpp
	src/mainwindowimpl.cpp
	src/rsschannel.cpp
	src/rsscontainer.cpp
	src/rssitem.cpp
	src/xmlparser.cpp
	src/rssitemtree.cpp
	src/httphandler.cpp
     ) 
SET( HDRS 
	src/mainwindowimpl.h
	src/rsschannel.h
	src/rsscontainer.h
	src/rssitem.h
	src/xmlparser.h
	src/qrssglobals.h
	src/rssitemtree.h
	src/httphandler.h
	src/httpobserver.h
	)
SET( UI_FORMS ui/mainwindow.ui )
QT4_WRAP_CPP(QT_MOC_SRC ${HDRS} )
QT4_WRAP_UI (UI_HDRS ${UI_FORMS} )
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR})
add_executable( QRss ${QT_MOC_SRC} ${SRCS} ${HDRS} ${UI_HDRS})
target_link_libraries( QRss ${QT_LIBRARIES} )