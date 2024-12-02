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
    std::vector<std::pair<float, float>> getPoints();
    std::vector<std::vector<std::pair<float, float>>> getLinestrings();
    std::vector<std::vector<std::vector<std::pair<float, float>>>> getPolygons();

private:
    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;

};

#endif // GEOJSONLOADER_H
