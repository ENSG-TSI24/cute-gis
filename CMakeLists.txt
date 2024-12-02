cmake_minimum_required(VERSION 3.5)

project(cute-gis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include ( CTest )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(GDAL REQUIRED)
find_package(CURL REQUIRED)
find_package(GTest REQUIRED)

include_directories(${GDAL_INCLUDE_DIR})
link_directories(${GDAL_LIBRARY_DIRS})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        src/API_WFS.cpp
        src/API_WFS.h
        src/API_WMS.cpp
        src/API_WMS.h
        src/API_WMTS.cpp
        src/API_WMTS.h
        src/DataProvider.h
        src/DataProvider.cpp

)

set(TEST_SOURCES
    tests/API_WMSTest.cpp
    src/API_WMS.h
    src/API_WMS.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cute-gis
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

qt_add_executable(API_WMSTest
    MANUAL_FINALIZATION
    ${TEST_SOURCES}
)


# Define target properties for Android with Qt 6 as:
#    set_property(TARGET cute-gis APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(cute-gis SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(cute-gis
            ${PROJECT_SOURCES})
        add_executable(API_WMSTest ${TEST_SOURCES})

    endif()
endif()

target_link_libraries(cute-gis  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets CURL::libcurl ${GDAL_LIBRARIES})
target_link_libraries(API_WMSTest PRIVATE GTest::GTest GTest::Main ${GDAL_LIBRARIES})

set_target_properties(cute-gis PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

enable_testing()
add_test(NAME API_WMSTest COMMAND API_WMSTest)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cute-gis)
endif()
