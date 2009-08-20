SET( TARGET ${PROJECT_NAME} )

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE( RAPI REQUIRED autolab-rapi  )
PKG_SEARCH_MODULE( STAGE stage )

IF( RAPI_FOUND )
  MESSAGE( STATUS ${INDENT} "Rapi version ${RAPI_VERSION} detected at ${RAPI_PREFIX}" )
  MESSAGE( STATUS "    RAPI_CFLAGS  = ${RAPI_CFLAGS}" )
  MESSAGE( STATUS "    RAPI_LDFLAGS = ${RAPI_LDFLAGS}" )
  MESSAGE( STATUS "    RAPI_INCLUDE = ${RAPI_INCLUDE_DIRS}" )
  MESSAGE( STATUS "    RAPI_LIBS    = ${RAPI_LIBRARIES}" )
ELSE( RAPI_FOUND )
  MESSAGE( ${INDENT} "Rapi not detected" )
ENDIF( RAPI_FOUND )

SET( SOURCES
  gridmap.cpp
  wavefrontmap.cpp
)
SET( HEADERS
  wavefrontmap.h
  gridmap.h
)

IF ( STAGE_FOUND )
  MESSAGE( STATUS "Stage found, will compile Stage dependencies")

  # do some work to cope with frameworks on OS X
  STRING(REGEX REPLACE ";" " " STAGE_LDFLAGS_MORE "${STAGE_LDFLAGS_OTHER}")
  MESSAGE(STAGE_LDFLAGS_MORE " ${STAGE_LDFLAGS_MORE}" )

  SET( SOURCES
     stagegridmap.cpp
     stagewavefrontmap.cpp
     ${SOURCES}
  )
  SET( HEADERS
    ${HEADERS}
    stagegridmap.h
    stagewavefrontmap.h
    wavefrontmapvis.h
  )
ELSE ( STAGE_FOUND )
  MESSAGE( STATUS "Stage was not found")
ENDIF ( STAGE_FOUND )



INCLUDE_DIRECTORIES(
  ${RAPI_INCLUDE_DIRS}
  ${STAGE_INCLUDE_DIRS}
)

LINK_DIRECTORIES( ${RAPI_LIBRARY_DIRS} ${STAGE_LIBRARY_DIRS} )

LINK_LIBRARIES( ${TARGET}
  ${RAPI_LIBRARIES}
  ${STAGE_LIBRARIES}
)

# build a shared library
ADD_LIBRARY( ${TARGET} SHARED ${SOURCES} )

# give the shared library a version number
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES
                       VERSION ${VERSION}
  		                 LINK_FLAGS "${STAGE_LDFLAGS_MORE}"
)

INSTALL(TARGETS ${TARGET}
  LIBRARY DESTINATION lib )

INSTALL(FILES ${HEADERS}
        DESTINATION include/${PROJECT_NAME}-${APIVERSION}
)
