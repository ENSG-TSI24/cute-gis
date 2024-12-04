cmake_minimum_required(VERSION 3.5)

project(cute-gis VERSION 0.1 LANGUAGES CXX)

include(CTest)
enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
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
    src/API_WMS.cpp
    src/API_WMS.h
    src/API_WMTS.cpp
    src/API_WMTS.h
    src/API_WFS.cpp
    src/API_WFS.h
    src/DataProvider.cpp
    src/DataProvider.h
)

set(TEST_SOURCES
    tests/API_WMSTest.cpp
    src/API_WMS.cpp
    src/API_WMS.h
    src/DataProvider.cpp
    src/DataProvider.h
)

set(TEST_SOURCES_WMTS
    tests/API_WMTSTest.cpp
    src/API_WMTS.cpp
    src/API_WMTS.h
    src/DataProvider.cpp
    src/DataProvider.h
)

set(TEST_SOURCES_WFS
    tests/API_WFSTest.cpp
    src/API_WFS.cpp
    src/API_WFS.h
    src/DataProvider.cpp
    src/DataProvider.h
)
add_executable(API_WMSTest ${TEST_SOURCES})
target_link_libraries(API_WMSTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets GTest::GTest GTest::Main CURL::libcurl ${GDAL_LIBRARIES})

add_executable(API_WFSTest ${TEST_SOURCES_WFS})
target_link_libraries(API_WFSTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets GTest::GTest GTest::Main CURL::libcurl ${GDAL_LIBRARIES})

add_executable(API_WMTSTest ${TEST_SOURCES_WMTS})
target_link_libraries(API_WMTSTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets GTest::GTest GTest::Main CURL::libcurl ${GDAL_LIBRARIES})

add_test(NAME API_WMSTest COMMAND API_WMSTest)
add_test(NAME API_WMTSTest COMMAND API_WMTSTest)
add_test(NAME API_WFSTest COMMAND API_WFSTest)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cute-gis
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(cute-gis ${PROJECT_SOURCES})
endif()

target_link_libraries(cute-gis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets CURL::libcurl ${GDAL_LIBRARIES})


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cute-gis)
endif()
