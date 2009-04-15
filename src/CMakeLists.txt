SET( TARGET ${PROJECT_NAME} )
SET( SOURCES 
     error.c
     common.cpp
     utilities.cpp
     gridmap.cpp
     wavefrontmap.cpp
     mapmarker.cpp
     logwriter.cpp
	  rangefinder.cpp
	  ccolor.cpp
)
SET( HEADERS
	  wavefrontmap.h
)
		  
# build a shared library
ADD_LIBRARY( ${TARGET} SHARED ${SOURCES} )

# give the shared library a version number
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES
		       VERSION ${VERSION}
)

INSTALL(TARGETS ${TARGET}
		  LIBRARY DESTINATION lib )

INSTALL(FILES ${HEADERS}
        DESTINATION include/${PROJECT_NAME}-${APIVERSION}
)
