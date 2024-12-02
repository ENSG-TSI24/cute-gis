
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
src/back/wfsdata.h
src/back/wfsdata.cpp
src/back/geotifffile.h src/back/geotifffile.cpp
src/back/geojsonfile.h src/back/geojsonfile.cpp
src/back/shpfile.h src/back/shpfile.cpp
src/back/citygmlfile.h src/back/citygmlfile.cpp
src/back/rasterdata.h src/back/rasterdata.cpp
src/back/DataManagement.h
src/back/abstractdata.h src/back/abstractdata.cpp
main.cpp
src/back/vectordata.h src/back/vectordata.cpp
)

set(TEST_SOURCES
    tests/testAbstractData.cpp
    tests/wfsdataTest.cpp
    src/back/DataManagement.h
    src/back/abstractdata.h src/back/abstractdata.cpp
    src/back/geojsonfile.h src/back/geojsonfile.cpp
    src/back/vectordata.h src/back/vectordata.cpp
    src/back/wfsdata.h
    src/back/wfsdata.cpp
)

add_executable(testAbstractData ${TEST_SOURCES})

add_executable(wfsdataTest ${TEST_SOURCES})

add_executable(cute-gis ${PROJECT_SOURCES})


target_link_libraries(cute-gis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${GDAL_LIBRARIES})

target_link_libraries(testAbstractData PRIVATE GTest::GTest GTest::Main ${GDAL_LIBRARIES})

target_link_libraries(wfsdataTest PRIVATE GTest::GTest GTest::Main ${GDAL_LIBRARIES})




enable_testing()

add_test(NAME testAbstractData COMMAND testAbstractData)

add_test(NAME wfsdataTest COMMAND wfsdataTest)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cute-gis)
endif()
