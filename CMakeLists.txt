project(VecHist)

cmake_minimum_required(VERSION 2.6)
# find_package(ANTTWEAKBAR REQUIRED)
if(UNIX)
set(CMAKE_MODULE_PATH "/usr/share/cmake-2.8/Modules")
set(CUDA_SDK_ROOT_DIR "/usr/local/cuda/samples")
endif()
find_package(OpenGL REQUIRED)
#find_package(EIGEN REQUIRED)
#find_package(LIBIGL REQUIRED)
#find_package(GLFW REQUIRED)
#find_package(GLEW)
find_package(CUDA REQUIRED)
find_package(OSUFlow REQUIRED)

#include_directories( ${ANT_TWEAK_BAR_INCLUDE_DIR} )
#message("ant:" ${ANT_TWEAK_BAR_INCLUDE_DIR})
#include_directories( ${LIBIGL_INCLUDE_DIR})
#include_directories( ${EIGEN_INCLUDE_DIR})
#include_directories( ${GLFW_INCLUDE_DIR})
#include_directories( ${GLEW_INCLUDE_DIR} )

find_package(Qt5Widgets)
#find_package(VTK REQUIRED)
#include(${VTK_USE_FILE})
#include_directories(${VTK_INCLUDE_DIRS})

#list( APPEND ${CMAKE_CXX_FLAGS} "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories( ${CUDA_INCLUDE_DIRS} ${CUDA_TOOLKIT_INCLUDE} ${CUDA_SDK_ROOT_DIR}/common/inc ${CMAKE_CURRENT_SOURCE_DIR} ${OSUFLOW_INCLUDE_DIR} required)

set(SHARED_LIBRARIES
#		${LIBIGL_LIBRARIES}
		${OPENGL_LIBRARIES}
#		${ANT_TWEAK_BAR_LIBRARY}
#		${EIGEN_LIBRARIES}
		# ${GLEW_LIBRARIES}
#		${GLFW_LIBRARIES}
)

#set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-O3 -gencode arch=compute_22,code=sm_22)
#set(GLEW_SOURCES "D:/lib/libigl/external/glew/src/glew.c")
#set(SHARED_SOURCES ${LIBIGL_SOURCES} ${GLEW_SOURCES})

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

#enable_cuda_support()

# qt5_wrap_ui(UI_Srcs ${UIs})
qt5_add_resources(QtApp_RCC_SRCS ${Rcs})
add_executable(VecHist ${Srcs} ${Hdrs} ${SHARED_SOURCES} ${QtApp_RCC_SRCS})#${UI_Srcs})# ${MOC_Hdrs})
# ${OPENGL_LIBRARIES}
qt5_use_modules(VecHist OpenGL Gui)
target_link_libraries(VecHist  Qt5::Widgets  ${SHARED_LIBRARIES} ${OSUFLOW_LIBRARY_DEBUG}) 
#opengl32
#target_compile_features(VecHist PRIVATE cxx_range_for)
# if(VTK_LIBRARIES)
  # target_link_libraries(VecHist ${VTK_LIBRARIES})
# else()
  # target_link_libraries(VecHist vtkHybrid vtkWidgets)
# endif()
