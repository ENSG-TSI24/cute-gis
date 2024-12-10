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

src/citygmlparser.h src/citygmlparser.cpp


)




add_executable(cute-gis ${PROJECT_SOURCES})


target_link_libraries(cute-gis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${GDAL_LIBRARIES})




enable_testing()
add_test(NAME wfsdataTest COMMAND wfsdataTest)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cute-gis)
endif()
