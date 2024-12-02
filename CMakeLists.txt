# Nom du projet
project(cute_GIS)

cmake_minimum_required(VERSION 3.5)

project(cute-gis VERSION 0.1 LANGUAGES CXX)

include ( CTest )

set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


find_package(GTest REQUIRED)
find_package(GDAL REQUIRED)

include_directories(${GDAL_INCLUDE_DIR})
link_directories(${GDAL_LIBRARY_DIRS})

set(PROJECT_SOURCES

src/geotifffile.h src/geotifffile.cpp
src/geojsonfile.h src/geojsonfile.cpp
src/shpfile.h src/shpfile.cpp
src/citygmlfile.h src/citygmlfile.cpp
src/rasterdata.h src/rasterdata.cpp
src/DataManagement.h
src/abstractdata.h src/abstractdata.cpp
src/main.cpp
src/vectordata.h src/vectordata.cpp
)

set(TEST_SOURCES
    tests/testAbstractData.cpp
    src/DataManagement.h
    src/abstractdata.h src/abstractdata.cpp
    src/geojsonfile.h src/geojsonfile.cpp
    src/vectordata.h src/vectordata.cpp
)

add_executable(testAbstractData ${TEST_SOURCES})
add_executable(cute-gis ${PROJECT_SOURCES})


target_link_libraries(cute-gis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${GDAL_LIBRARIES})
target_link_libraries(testAbstractData PRIVATE GTest::GTest GTest::Main ${GDAL_LIBRARIES})



enable_testing()
add_test(NAME testAbstractData COMMAND testAbstractData)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cute-gis)
endif()
