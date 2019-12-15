project(ferguson)

cmake_minimum_required(VERSION 3.12.0)

# Qt Library
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets OpenGL)
set(CMAKE_AUTOMOC ON)

# OpenGL
find_package(OpenGL)

set(SOURCES 
	main.cpp
	renderer.cpp
	window.cpp
	ferguson_canvas.cpp
	ferguson_patch.cpp
	inner_point_control.cpp
	ferguson_control.cpp)

#executable
add_executable(ferguson ${SOURCES})
target_link_libraries(ferguson PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL)