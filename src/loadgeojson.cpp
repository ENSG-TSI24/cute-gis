#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

#include "loadgeojson.h"


LoadGeojson::LoadGeojson() {

}

LoadGeojson::~LoadGeojson() {
}

bool LoadGeojson::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return false;
    }

    file >> geojsonData;

    return true;
}