cmake_minimum_required(VERSION 3.5)

project(cute-gis LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

add_executable(cute-gis main.cpp mainwindow.cpp mainwindow.h mainwindow.ui
        triangle.h triangle.cpp geojsonviewer.h geojsonviewer.cpp camera.h camera.cpp)

target_link_libraries(cute-gis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::OpenGLWidgets glfw GLEW libGLEW.so libGLU.so libGL.so)
