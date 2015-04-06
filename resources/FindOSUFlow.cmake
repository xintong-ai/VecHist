#

# Try to find OSUFlow library and include path.

# Once done this will define

#

# OSUFLOW_FOUND

# OSUFLOW_INCLUDE_DIR

# OSUFLOW_LIBRARY

#

if(WIN32)
FIND_PATH(OSUFLOW_INCLUDE_DIR OSUFlow.h
  PATHS
    D:\Dropbox\hist\OSUFlow\src
    )

elseif(UNIX)
FIND_PATH(OSUFLOW_INCLUDE_DIR OSUFlow.h
  PATHS
		$ENV{HOME}/Dropbox/hist/OSUFlow/src
    )
endif()





if(WIN32)

FIND_LIBRARY( OSUFLOW_LIBRARY NAMES OSUFlow.lib
  PATHS
	D:\Dropbox\hist\OSUFlow-build\src\Release
)

elseif(UNIX)

FIND_LIBRARY( OSUFLOW_LIBRARY NAMES libOSUFlow.a
  PATHS
	$ENV{HOME}/Dropbox/hist/OSUFlow-linux/src
)

endif()

if(WIN32)

FIND_LIBRARY( OSUFLOW_LIBRARY_DEBUG NAMES OSUFlow_d.lib
  PATHS
	D:\Dropbox\hist\OSUFlow-build\src\Debug
)

elseif(UNIX)

FIND_LIBRARY( OSUFLOW_LIBRARY_DEBUG NAMES libOSUFlow.a
  PATHS
	$ENV{HOME}/Dropbox/hist/OSUFlow-linux/src
)

endif()



SET(OSUFLOW_FOUND "NO")

IF (OSUFLOW_INCLUDE_DIR AND OSUFLOW_LIBRARY)

	SET(OSUFLOW_FOUND "YES")

ENDIF (OSUFLOW_INCLUDE_DIR AND OSUFLOW_LIBRARY)



if(OSUFLOW_FOUND)

  message(STATUS "Found OSUFLOW: ${OSUFLOW_INCLUDE_DIR}")

else(OSUFLOW_FOUND)

  message(FATAL_ERROR "could NOT find OSUFLOW")

endif(OSUFLOW_FOUND)

