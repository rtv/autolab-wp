
		  
# build a shared library
ADD_LIBRARY( autolab-wp SHARED
            error.c
            utilities.cpp
            common.cpp
#            error.c
            utilities.cpp
            gridmap.cpp
            wavefrontmap.cpp
            mapmarker.cpp
            logwriter.cpp
				rangefinder.cpp
				ccolor.cpp
           )


#TARGET_LINK_LIBRARIES( fasrrobot
#  ${OPENGL_LIBRARIES}
#)

INSTALL(TARGETS autolab-wp
	LIBRARY DESTINATION lib
)
