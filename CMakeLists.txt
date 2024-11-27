cmake_minimum_required(VERSION 3.5)

# Nom du projet
project(cute_GIS)

# Définit le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Trouver la bibliothèque GDAL
find_package(GDAL REQUIRED)

# Inclure les headers et lier les bibliothèques GDAL
include_directories(${GDAL_INCLUDE_DIR})
link_directories(${GDAL_LIBRARY_DIRS})

# Ajouter le fichier source principal
add_executable(test_gdal src/vectordata.h src/vectordata.cpp src/geotifffile.h src/geotifffile.cpp src/geojsonfile.h src/geojsonfile.cpp src/geojsonfile.h src/geojsonfile.cpp src/shpfile.h src/shpfile.cpp src/citygmlfile.h src/citygmlfile.cpp src/rasterdata.h src/rasterdata.cpp src/DataManagement.h src/abstractdata.h src/abstractdata.cpp)

# Lier l'exécutable avec GDAL
target_link_libraries(test_gdal ${GDAL_LIBRARIES})

