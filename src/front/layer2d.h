#ifndef LAYER2D_H
#define LAYER2D_H

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <QJsonObject>
#include "../back/vectordata.h"
#include "controller.h"

class Layer2d
{
public:
    Layer2d(VectorData data, const std::string& geoJsonPath = "");
    ~Layer2d();
    void renderPoints();
    void renderLinestrings();
    void renderPolygons();
    void calculateBoundingBox();

    BoundingBox boundingBox;
    bool isVisible = true;

    std::string name = "no name";
    std::string geoJsonPath;
    std::vector<QJsonObject> properties;

    bool isHighlighted() const { return highlightedFlag; }
    void setHighlighted(bool value) { highlightedFlag = value; }

private:
    bool highlightedFlag = false;  
    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;
};

#endif // LAYER2D_H

