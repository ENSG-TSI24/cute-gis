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
find_package(jsoncpp REQUIRED)
find_package(GTest REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        src/loadgeojson.h
        src/loadgeojson.cpp
)
set(TEST_SOURCES
    tests/loadgeojsonTest.cpp
)
add_executable(loadgeojsonTest ${TEST_SOURCES})
qt_add_executable(cute-gis
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )


target_link_libraries(cute-gis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets jsoncpp_lib)
target_link_libraries(loadgeojsonTest PRIVATE GTest::GTest GTest::Main jsoncpp_lib)


set_target_properties(cute-gis PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)



enable_testing()
add_test(NAME loadgeojsonTest COMMAND loadgeojsonTest)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cute-gis)
endif()