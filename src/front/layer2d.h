#ifndef LAYER2D_H
#define LAYER2D_H

#include <vector>
#include <utility>
#include "../back/vectordata.h"
#include "controller.h"

class Layer2d
{
public:
    Layer2d(VectorData data);
    ~Layer2d();
    void renderPoints();
    void renderLinestrings();
    void renderPolygons();
    void calculateBoundingBox();
    // Highlight specific geometry
    void highlightGeometry(int rowIndex);

    // Properties
    BoundingBox boundingBox;
    int highlightedIndex = -1;
    bool isVisible = true;
    std::string name = "no name";
    std::vector<std::vector<std::string>> attributes;
    std::vector<std::string> attributeHeaders;


        float opacity = 1.0f;

private:
    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::tuple<float, float, float>>>> polygons;
    std::vector<std::vector<std::vector<std::pair<float, float>>> > polygons2d;
};

#endif // LAYER2D_H
