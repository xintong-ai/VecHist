project(VecHist)

cmake_minimum_required(VERSION 2.6)
find_package(OpenGL REQUIRED)
find_package(OSUFlow REQUIRED)
find_package(Qt5Widgets)
find_package(VTK REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${OSUFLOW_INCLUDE_DIR} ${VTK_INCLUDE_DIRS} required)

set(SHARED_LIBRARIES
		${OPENGL_LIBRARIES}
		${OSUFLOW_LIBRARY_DEBUG}
		${VTK_LIBRARY_DIRS}
)

message("OSUFLOW_LIBRARY_DEBUG")
message("${OSUFLOW_LIBRARY_DEBUG}")

message("VTK LIBRARY DIRS:")
message("${VTK_LIBRARY_DIRS}")

source_group("widgets_h" FILES edge.h node.h graphwidget.h qjson.h qjsonview.h GraphVizWidget.h EntropySlider.h TreeMapPlot.h TreeMapWindow.h)

source_group("widgets_cpp" FILES edge.cpp node.cpp graphwidget.cpp qjson.cpp qjsonview.cpp GraphVizWidget.cpp EntropySlider.cpp  TreeMapPlot.cpp TreeMapWindow.cpp)

set( Srcs main.cpp DataManager.cpp glsphere.cpp glSuperquadric.cpp
	scene.cpp glbuffers.cpp glextensions.cpp trackball.cpp edge.cpp graphwidget.cpp node.cpp qjson.cpp qjsonview.cpp
	DataMgrCosm.cpp DataMgrVect.cpp EntropySlider.cpp TreeMapPlot.cpp TreeMapWindow.cpp TextureCubeManager.cpp
	)
set( Hdrs DataManager.h glsphere.h glSuperquadric.h gltrianglemesh.h
	scene.h glbuffers.h glextensions.h trackball.h edge.h graphwidget.h node.h
	DataMgrCosm.h DataMgrVect.h qjson.h qjsonview.h EntropySlider.h TreeMapPlot.h TreeMapWindow.h TextureCubeManager.h
    )
set( Rcs resources/vechist.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

qt5_add_resources(QtApp_RCC_SRCS ${Rcs})
add_executable(VecHist ${Srcs} ${Hdrs} ${QtApp_RCC_SRCS})
qt5_use_modules(VecHist OpenGL Gui)
target_link_libraries(VecHist  Qt5::Widgets  ${SHARED_LIBRARIES} )
