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

include_directories(${GDAL_INCLUDE_DIR})
link_directories(${GDAL_LIBRARY_DIRS})

set(PROJECT_SOURCES
    main.cpp
    # Backend files
    src/back/wfsdata.h src/back/wfsdata.cpp
    src/back/citygmlfile.h src/back/citygmlfile.cpp
    src/back/rasterdata.h src/back/rasterdata.cpp
    src/back/DataManagement.h
    src/back/dataextract/abstractdatavisitor.h src/back/dataextract/abstractdatavisitor.cpp
    src/back/dataextract/geojsonvisitor.h src/back/dataextract/geojsonvisitor.cpp
    src/back/dataextract/intersectvisitor.h src/back/dataextract/intersectvisitor.cpp
    src/back/vectordata.h src/back/vectordata.cpp
    # Frontend files
    src/front/mainwindow.cpp src/front/mainwindow.h src/front/mainwindow.ui
    src/front/camera.h src/front/camera.cpp
    src/front/controller.h src/front/controller.cpp
    src/front/geojsonloader.h src/front/geojsonloader.cpp
    src/front/renderer.h src/front/renderer.cpp
    src/front/objectloader.h src/front/objectloader.cpp
    src/lib/tiny_obj_loader.h src/lib/tiny_obj_loader.cc
    src/front/layer2d.h src/front/layer2d.cpp
)

set(TEST_SOURCES
    tests/testAbstractData.cpp
    tests/wfsdataTest.cpp
    src/back/DataManagement.h
    src/back/abstractdata.h src/back/abstractdata.cpp
    src/back/vectordata.h src/back/vectordata.cpp
    src/back/wfsdata.h src/back/wfsdata.cpp
)

# Backend tests
add_executable(testAbstractData ${TEST_SOURCES})
add_executable(wfsdataTest ${TEST_SOURCES})

# Main application
qt_add_executable(cute-gis
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Link libraries
target_link_libraries(cute-gis PRIVATE Qt6::Widgets Qt6::OpenGLWidgets glfw GLEW libGLEW.so libGLU.so libGL.so ${GDAL_LIBRARIES})
target_link_libraries(testAbstractData PRIVATE GTest::GTest GTest::Main ${GDAL_LIBRARIES})
target_link_libraries(wfsdataTest PRIVATE GTest::GTest GTest::Main ${GDAL_LIBRARIES})

# Finalize application
qt_finalize_executable(cute-gis)

# Enable testing
enable_testing()
add_test(NAME testAbstractData COMMAND testAbstractData)
add_test(NAME wfsdataTest COMMAND wfsdataTest)
