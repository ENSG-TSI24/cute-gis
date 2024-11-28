cmake_minimum_required(VERSION 3.5)

project(cute-gis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        camera.h camera.cpp
        controller.h controller.cpp
        geojsonloader.h geojsonloader.cpp
        renderer.h renderer.cpp
        objectloader.h objectloader.cpp
        tiny_obj_loader.h tiny_obj_loader.cc
)

qt_add_executable(cute-gis
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(cute-gis PRIVATE Qt6::Widgets Qt6::OpenGLWidgets glfw GLEW libGLEW.so libGLU.so libGL.so)

set_target_properties(cute-gis PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(cute-gis)
