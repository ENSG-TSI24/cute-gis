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

# Trouver GDAL
find_package(GDAL REQUIRED)

set(PROJECT_SOURCES
        texturegeotif.h texturegeotif.cpp
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
        geotifloader.h geotifloader.cpp

)

qt_add_executable(cute-gis
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(cute-gis PRIVATE Qt6::Widgets Qt6::OpenGLWidgets glfw GLEW libGLEW.so libGLU.so libGL.so ${GDAL_LIBRARIES})

# Inclure les répertoires d'en-têtes GDAL
target_include_directories(cute-gis PRIVATE ${GDAL_INCLUDE_DIRS})

set_target_properties(cute-gis PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Affichage des informations GDAL pour vérifier que tout fonctionne
message(STATUS "GDAL include dirs: ${GDAL_INCLUDE_DIRS}")
message(STATUS "GDAL libraries: ${GDAL_LIBRARIES}")

qt_finalize_executable(cute-gis)
