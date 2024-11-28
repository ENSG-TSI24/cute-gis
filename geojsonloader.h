#ifndef GEOJSONLOADER_H
#define GEOJSONLOADER_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>


class Geojsonloader
{
public:
    Geojsonloader(const std::string& filePath);
    std::vector<std::pair<float, float>> getCoordinates();


private:
        std::vector<std::pair<float, float>> coordinates;

};

#endif // GEOJSONLOADER_H
