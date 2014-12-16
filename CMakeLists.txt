project(VecHist)

cmake_minimum_required(VERSION 2.6)
# find_package(ANTTWEAKBAR REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(EIGEN REQUIRED)
#find_package(LIBIGL REQUIRED)
#find_package(GLFW REQUIRED)
#find_package(GLEW)
find_package(CUDA)

#include_directories( ${ANT_TWEAK_BAR_INCLUDE_DIR} )
#message("ant:" ${ANT_TWEAK_BAR_INCLUDE_DIR})
#include_directories( ${LIBIGL_INCLUDE_DIR})
#include_directories( ${EIGEN_INCLUDE_DIR})
#include_directories( ${GLFW_INCLUDE_DIR})
#include_directories( ${GLEW_INCLUDE_DIR} )
include_directories( ${CUDA_INCLUDE_DIRS} )

set(SHARED_LIBRARIES
#		${LIBIGL_LIBRARIES}
		${OPENGL_LIBRARIES}
#		${ANT_TWEAK_BAR_LIBRARY}
#		${EIGEN_LIBRARIES}
		# ${GLEW_LIBRARIES}
#		${GLFW_LIBRARIES}
)

#set(GLEW_SOURCES "D:/lib/libigl/external/glew/src/glew.c")
#set(SHARED_SOURCES ${LIBIGL_SOURCES} ${GLEW_SOURCES})

# Set your files and resources here
set( Srcs glwidget.cpp main.cpp window.cpp DataManager.cpp glsphere.cpp
	scene.cpp glbuffers.cpp glextensions.cpp trackball.cpp )# 3rdparty/fbm.c) # box.cpp
set( Hdrs glwidget.h window.h DataManager.h vector_types.h glsphere.h
	scene.h glbuffers.h glextensions.h trackball.h gltrianglemesh.h )# 3rdparty/fbm.h) #box.h 
set( Rcs resources/boxes.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
# qt5_wrap_ui(UI_Srcs ${UIs})
qt5_add_resources(QtApp_RCC_SRCS ${Rcs})
add_executable(VecHist ${Srcs} ${Hdrs} ${SHARED_SOURCES} ${QtApp_RCC_SRCS})#${UI_Srcs})# ${MOC_Hdrs})
qt5_use_modules(VecHist OpenGL GUI)
target_link_libraries(VecHist  Qt5::Widgets opengl32.lib ${SHARED_LIBRARIES}) 
