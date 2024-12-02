cmake_minimum_required(VERSION 3.5)

project(cute-gis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(GDAL REQUIRED)

find_package(CURL REQUIRED)


include_directories(${GDAL_INCLUDE_DIR})

link_directories(${GDAL_LIBRARY_DIRS})

set(PROJECT_SOURCES

        main.cpp

        mainwindow.cpp

        mainwindow.h

        mainwindow.ui

        src/API_WMS.cpp
        src/API_WMS.h

        src/API_WMTS.h
        src/API_WMTS.cpp

        src/API_WFS.cpp
        src/API_WFS.h

        src/DataProvider.h
        src/DataProvider.cpp

)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(cute-gis

        MANUAL_FINALIZATION

        ${PROJECT_SOURCES}
    )

    endif()


if(QT_VERSION_MAJOR EQUAL 6)

        qt_finalize_executable(cute-gis)

    endif()


target_link_libraries(cute-gis  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets CURL::libcurl ${GDAL_LIBRARIES})
