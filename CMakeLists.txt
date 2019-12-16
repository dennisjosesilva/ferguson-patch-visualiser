project(ferguson)

cmake_minimum_required(VERSION 3.12.0)

# Qt Library
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets OpenGL)
set(CMAKE_AUTOMOC ON)

# OpenGL
find_package(OpenGL)

include_directories(include)

set(SOURCES 
	./src/main.cpp
	./src/renderer.cpp
	./src/window.cpp
	./src/ferguson_canvas.cpp
	./src/ferguson_patch.cpp
	./src/inner_point_control.cpp
	./src/ferguson_control.cpp)

#executable
add_executable(ferguson ${SOURCES})
target_link_libraries(ferguson PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL)