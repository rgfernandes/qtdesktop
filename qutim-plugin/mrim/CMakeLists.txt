CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (mrim)

LIST (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

file( GLOB_RECURSE MRIM_H coresrc/*.h uisrc/*.h)
file( GLOB_RECURSE MRIM_CPP coresrc/*.cpp uisrc/*.cpp)
file( GLOB_RECURSE MRIM_UI uisrc/*.ui)

# message( "MRIM C++ code: ${MRIM_CPP}" )
# message( "MRIM Headers: ${MRIM_H}" )

message( "INSTALL DIRECTORY: ${CMAKE_INSTALL_PREFIX} " )

include( CheckFunctionExists )

SET (MRIM_LIBS "")
SET (MRIM_INCLUDE_DIR "")

###For testing purposes, enable verbose logging in debug builds
IF (  CMAKE_BUILD_TYPE STREQUAL "Debug" )	
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_LEVEL_DEV")
	MESSAGE( "Verbose logging enabled!" )
ENDIF ( CMAKE_BUILD_TYPE STREQUAL "Debug"  )
###

SET (QT_USE_QTNETWORK true)
SET (QT_USE_QTGUI true)
FIND_PACKAGE (Qt4 REQUIRED)
SET(QT_MIN_VERSION "4.4.0")

INCLUDE (${CMAKE_ROOT}/Modules/UseQt4.cmake)
FIND_PATH( QUTIM_INCLUDE_DIRS qutim/protocolinterface.h ../../include ../../../include)
if( QUTIM_INCLUDE_DIRS )
	MESSAGE( STATUS "Found qutIM headers: ${QUTIM_INCLUDE_DIRS}" )
	INCLUDE_DIRECTORIES( ${QUTIM_INCLUDE_DIRS} )
else( QUTIM_INCLUDE_DIRS )
	MESSAGE( SEND_ERROR "Could NOT find qutIM development headers" )
endif( QUTIM_INCLUDE_DIRS )
if( APPLE )
	INCLUDE_DIRECTORIES ( /opt/local/include )
endif( APPLE )
if( UNIX )
INCLUDE_DIRECTORIES ( /usr/include )
endif( UNIX )

INCLUDE_DIRECTORIES (${QT_QTGUI_INCLUDE_DIR}
        ${QT_QTCORE_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
	${MRIM_INCLUDE_DIR}
        .
                coresrc
                uisrc
        )

SET(HEADERS
${MRIM_H}
)

SET (SOURCES
${MRIM_CPP}
)

SET (RESOURCES
./mrim.qrc
)

SET (FORMS
${MRIM_UI}
)

QT4_WRAP_CPP (MOC_SRCS ${MRIM_H})
QT4_WRAP_UI (UIS_H ${FORMS})
QT4_ADD_RESOURCES (RCC ${RESOURCES})

ADD_LIBRARY (mrim SHARED ${SOURCES} ${MOC_SRCS} ${UIS_H} ${RCC})

TARGET_LINK_LIBRARIES (mrim
        ${QT_LIBRARIES}
        )

if ( !WIN32 )
INSTALL (TARGETS mrim DESTINATION lib/qutim )
endif ( !WIN32 )

if ( WIN32 )
INSTALL (TARGETS mrim DESTINATION ../qutim/lib/qutim )
endif ( WIN32 )
