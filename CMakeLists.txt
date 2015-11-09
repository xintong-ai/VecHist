project(VecHist)

cmake_minimum_required(VERSION 2.6)

Set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x64")

find_package(OpenGL REQUIRED)
#find_package(OSUFlow REQUIRED)
find_package(Qt5Widgets)
#find_package(VTK REQUIRED)
find_package(CUDA REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#include(${VTK_USE_FILE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} 
#${OSUFLOW_INCLUDE_DIR} ${VTK_INCLUDE_DIRS} 
	${CUDA_TOOLKIT_INCLUDE} ${CUDA_SDK_ROOT_DIR}/common/inc required)

set(SHARED_LIBRARIES
		${OPENGL_LIBRARIES}
#		${VTK_LIBRARIES}
#		debug ${OSUFLOW_LIBRARY_DEBUG}
#		optimized ${OSUFLOW_LIBRARY}
)

#message("OSUFLOW_LIBRARY_DEBUG")
#message("${OSUFLOW_LIBRARY_DEBUG}")

#message("VTK LIBRARY DIRS:")
#message("${VTK_LIBRARY_DIRS}")

#source_group("widgets_h" FILES edge.h node.h graphwidget.h qjson.h qjsonview.h GraphVizWidget.h EntropySlider.h TreeMapPlot.h TreeMapWindow.h ArrowButton.h ArrowWidget.h EntropyQueryContainer.h)

#source_group("widgets_cpp" FILES edge.cpp node.cpp graphwidget.cpp qjson.cpp qjsonview.cpp GraphVizWidget.cpp EntropySlider.cpp  TreeMapPlot.cpp TreeMapWindow.cpp ArrowButton.cpp ArrowWidget.cpp EntropyQueryContainer.cpp)

set( Srcs main.cpp GLSphere.cpp
	window.cpp glwidget.cpp Renderable.cpp Trackball.cpp Rotation.cpp QuatVector.cpp
	DataMgr.cpp Reader.cpp VecReader.cpp Cubemap.cpp 
	#Streamline.cpp LineRenderable.cpp 
	BoxRenderable.cpp GlyphRenderable.cpp MeshReader.cpp
	GLTextureCube.cpp GL2DProjWidget.cpp
	)
set( Hdrs 
GLSphere.h
	window.h glwidget.h Renderable.h Trackball.h Rotation.h QuatVector.h
	DataMgr.h defines.h Reader.h VecReader.h Cubemap.h 
	#Streamline.h LineRenderable.h 
	BoxRenderable.h ShaderProgram.h GlyphRenderable.h MeshReader.h
	GLTextureCube.h GL2DProjWidget.h
    )
set( Rcs resources/vechist.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

qt5_add_resources(QtApp_RCC_SRCS ${Rcs})
add_executable(VecHist ${Srcs} ${Hdrs} ${QtApp_RCC_SRCS})
qt5_use_modules(VecHist OpenGL Gui)
target_link_libraries(VecHist  Qt5::Widgets  ${SHARED_LIBRARIES} )
