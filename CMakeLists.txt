cmake_minimum_required(VERSION 3.5)

project(cute-gis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets Network)
find_package(GDAL REQUIRED)
find_package(CURL REQUIRED)

# Include the build directory for generated header files
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${GDAL_INCLUDE_DIR})
link_directories(${GDAL_LIBRARY_DIRS})

# Add your project sources
set(PROJECT_SOURCES
    main.cpp
    src/front/mainwindow.cpp
    src/front/mainwindow.h
    src/front/mainwindow.ui
    src/front/camera.h
    src/front/camera.cpp
    src/front/controller.h
    src/front/controller.cpp
    src/front/geojsonloader.h
    src/front/geojsonloader.cpp
    src/front/renderer.h
    src/front/renderer.cpp
    src/front/objectloader.h
    src/front/objectloader.cpp
    src/front/addFluxData.cpp
    src/front/addFluxData.h
    src/front/addFluxData.ui
    src/lib/tiny_obj_loader.h
    src/lib/tiny_obj_loader.cc

    src/back/API_WMS.h src/back/API_WMS.cpp
    src/back/API_WMTS.h src/back/API_WMTS.cpp
    src/back/API_WFS.h src/back/API_WFS.cpp
    src/back/DataProvider.h src/back/DataProvider.cpp
)

# Wrap the .ui files
qt_wrap_ui(UI_HEADERS
    src/front/mainwindow.ui
    src/front/addFluxData.ui
)

# Define the executable
qt_add_executable(cute-gis
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${UI_HEADERS}
)

# Link necessary libraries
target_link_libraries(cute-gis PRIVATE Qt6::Widgets Qt6::OpenGLWidgets Qt6::Network glfw GLEW libGLEW.so libGLU.so libGL.so)
target_link_libraries(cute-gis PRIVATE Qt6::Widgets Qt6::OpenGLWidgets glfw GLEW libGLEW.so libGLU.so libGL.so CURL::libcurl ${GDAL_LIBRARIES})

# Set target properties
set_target_properties(cute-gis PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Finalize the executable
qt_finalize_executable(cute-gis)
