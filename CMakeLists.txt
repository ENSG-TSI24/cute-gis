cmake_minimum_required(VERSION 3.5)

project(cute-gis VERSION 0.1 LANGUAGES CXX)

include ( CTest )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
message(STATUS "Using Qt version ${QT_VERSION_MAJOR}")
find_package(GTest REQUIRED)
find_package(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR})
link_directories(${GDAL_LIBRARY_DIRS})

set(PROJECT_SOURCES
        main.cpp
        src/wfsdata.h
        src/wfsdata.cpp
)
set(TEST_SOURCES
    tests/wfsdataTest.cpp
    src/wfsdata.h
    src/wfsdata.cpp
    src/geotifffile.h src/geotifffile.cpp
    src/geojsonfile.h src/geojsonfile.cpp
    src/geojsonfile.h src/geojsonfile.cpp
    src/shpfile.h src/shpfile.cpp
    src/citygmlfile.h src/citygmlfile.cpp
    src/rasterdata.h src/rasterdata.cpp
    src/DataManagement.h
    src/abstractdata.h src/abstractdata.cpp
    src/main.cpp
)
add_executable(wfsdataTest ${TEST_SOURCES})
qt_add_executable(cute-gis
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )


target_link_libraries(cute-gis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${GDAL_LIBRARIES})
target_link_libraries(wfsdataTest PRIVATE GTest::GTest GTest::Main ${GDAL_LIBRARIES})


set_target_properties(cute-gis PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)



enable_testing()
add_test(NAME wfsdataTest COMMAND wfsdataTest)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cute-gis)
endif()