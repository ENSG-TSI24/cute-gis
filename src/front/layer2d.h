#ifndef LAYER2D_H
#define LAYER2D_H

#include <vector>
#include <utility>
#include "geojsonloader.h"
#include "controller.h"



class Layer2d
{
public:
    Layer2d(Geojsonloader data);
    Layer2d(Geojsonloader data, std::string name);
    ~Layer2d();
    void renderPoints();
    void renderLinestrings();
    void renderPolygons();
    void calculateBoundingBox();
    BoundingBox boundingBox;
    void reName(std::string name);
    std::string getName();

private:
    std::string name;
    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;
};

#endif // LAYER2D_H
