cmake_minimum_required(VERSION 3.5)

# Nom du projet
project(test_conv_shp2gjson LANGUAGES CXX)

# Configuration C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver les bibliothèques nécessaires
find_package(Qt6 COMPONENTS Core REQUIRED) # Remplacez par Qt5 si nécessaire
find_package(GDAL REQUIRED)

# Ajouter l'exécutable
add_executable(test_conv_shp2gjson main.cpp)

# Lier les bibliothèques
target_link_libraries(test_conv_shp2gjson PRIVATE Qt6::Core GDAL::GDAL)
