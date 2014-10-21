project(VecHist)

cmake_minimum_required(VERSION 2.6)
find_package(ANTTWEAKBAR REQUIRED)
find_package(OpenGL REQUIRED)
find_package(EIGEN REQUIRED)
find_package(LIBIGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW)
include_directories( ${ANT_TWEAK_BAR_INCLUDE_DIR} )
#message("ant:" ${ANT_TWEAK_BAR_INCLUDE_DIR})
include_directories( ${LIBIGL_INCLUDE_DIR})
include_directories( ${EIGEN_INCLUDE_DIR})
include_directories( ${GLFW_INCLUDE_DIR})
include_directories( ${GLEW_INCLUDE_DIR} )

set(SHARED_LIBRARIES
		${LIBIGL_LIBRARIES}
		${OPENGL_LIBRARIES}
		${ANT_TWEAK_BAR_LIBRARY}
		${EIGEN_LIBRARIES}
		# ${GLEW_LIBRARIES}
		${GLFW_LIBRARIES}
)

set(SHARED_SOURCES ${LIBIGL_SOURCES})
set(GLEW_SOURCES "D:/lib/libigl/external/glew/src/glew.c")
set(SHARED_SOURCES ${SHARED_SOURCES} ${GLEW_SOURCES})
message(SHARED_SOURCES ${SHARED_SOURCES})
# if("${VTK_QT_VERSION}" STREQUAL "")
  # message(FATAL_ERROR "VTK was not built with Qt")
# endif()

# Set your files and resources here
set( Srcs glwidget.cpp main.cpp window.cpp hist.cpp DataManager.cpp)
set( Hdrs glwidget.h window.h hist.h dstat.h DataManager.h)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
# qt5_wrap_ui(UI_Srcs ${UIs})
add_executable(VecHist ${Srcs} ${Hdrs} ${SHARED_SOURCES})#${UI_Srcs})# ${MOC_Hdrs})
qt5_use_modules(VecHist OpenGL)
target_link_libraries(VecHist  Qt5::Widgets opengl32.lib ${SHARED_LIBRARIES}) 
