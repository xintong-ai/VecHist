project(VecHist)

cmake_minimum_required(VERSION 2.6)
if(UNIX)
############set(CMAKE_MODULE_PATH "/usr/share/cmake-2.8/Modules")
set(CUDA_SDK_ROOT_DIR "/usr/local/cuda/samples")
endif()
find_package(OpenGL REQUIRED)
find_package(CUDA REQUIRED)
find_package(OSUFlow REQUIRED)
find_package(Qt5Widgets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories( ${CUDA_INCLUDE_DIRS} ${CUDA_TOOLKIT_INCLUDE} ${CUDA_SDK_ROOT_DIR}/common/inc ${CMAKE_CURRENT_SOURCE_DIR} ${OSUFLOW_INCLUDE_DIR} required)

set(SHARED_LIBRARIES
		${OPENGL_LIBRARIES}
		${OSUFLOW_LIBRARY_DEBUG} 
)

# Set your files and resources here
set( Srcs main.cpp DataManager.cpp glsphere.cpp glSuperquadric.cpp
	scene.cpp glbuffers.cpp glextensions.cpp trackball.cpp 
	)# 3rdparty/fbm.c) # box.cpp
set( Hdrs DataManager.h glsphere.h glSuperquadric.h gltrianglemesh.h
	scene.h glbuffers.h glextensions.h trackball.h 
    )# 3rdparty/fbm.h) #box.h 
set( Rcs resources/boxes.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# qt5_wrap_ui(UI_Srcs ${UIs})
qt5_add_resources(QtApp_RCC_SRCS ${Rcs})
add_executable(VecHist ${Srcs} ${Hdrs} ${SHARED_SOURCES} ${QtApp_RCC_SRCS})#${UI_Srcs})# ${MOC_Hdrs})
# ${OPENGL_LIBRARIES}
qt5_use_modules(VecHist OpenGL Gui)
message("${osuflow_LIBRARIES}")
target_link_libraries(VecHist  Qt5::Widgets  ${SHARED_LIBRARIES} )
#opengl32
