cmake_minimum_required(VERSION 3.5)

project(cute-gis VERSION 0.1 LANGUAGES CXX)

include ( CTest )
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

find_package(GTest REQUIRED)
find_package(GDAL REQUIRED)
find_package(CURL REQUIRED)

include_directories(${GDAL_INCLUDE_DIR})
link_directories(${GDAL_LIBRARY_DIRS})

set(PROJECT_SOURCES
    main.cpp
    # Backend files
    src/back/API_WMS.h src/back/API_WMS.cpp
    src/back/API_WMTS.h src/back/API_WMTS.cpp
    src/back/API_WFS.h src/back/API_WFS.cpp
    src/back/DataProvider.h src/back/DataProvider.cpp
    # Frontend files

)

set(TEST_SOURCES
    tests/API_WMSTest.cpp
    src/back/API_WMS.cpp
    src/back/API_WMS.h
    src/back/DataProvider.cpp
    src/back/DataProvider.h
)

set(TEST_SOURCES_WMTS
    tests/API_WMTSTest.cpp
    src/back/API_WMTS.cpp
    src/back/API_WMTS.h
    src/back/DataProvider.cpp
    src/back/DataProvider.h
)

set(TEST_SOURCES_WFS
    tests/API_WFSTest.cpp
    src/back/API_WFS.cpp
    src/back/API_WFS.h
    src/back/DataProvider.cpp
    src/back/DataProvider.h
)

# Backend tests
add_executable(API_WMSTest ${TEST_SOURCES})
add_executable(API_WFSTest ${TEST_SOURCES_WFS})
add_executable(API_WMTSTest ${TEST_SOURCES_WMTS})

# Main application
qt_add_executable(cute-gis
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Link libraries
target_link_libraries(cute-gis PRIVATE Qt6::Widgets Qt6::OpenGLWidgets glfw GLEW libGLEW.so libGLU.so libGL.so CURL::libcurl ${GDAL_LIBRARIES})
target_link_libraries(API_WMSTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets GTest::GTest GTest::Main CURL::libcurl ${GDAL_LIBRARIES})
target_link_libraries(API_WFSTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets GTest::GTest GTest::Main CURL::libcurl ${GDAL_LIBRARIES})
target_link_libraries(API_WMTSTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets GTest::GTest GTest::Main CURL::libcurl ${GDAL_LIBRARIES})


# Finalize application
qt_finalize_executable(cute-gis)

# Enable testing
enable_testing()
add_test(NAME testAbstractData COMMAND testAbstractData)
add_test(NAME wfsdataTest COMMAND wfsdataTest)
