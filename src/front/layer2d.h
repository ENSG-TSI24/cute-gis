#ifndef LAYER2D_H
#define LAYER2D_H

#include <vector>
#include <utility>
#include "../back/vectordata.h"
#include "boundingbox.h"
#include "layerbase.h"

class Layer2d : public LayerBase
{
public:
    explicit Layer2d(VectorData data);
    ~Layer2d() override;
    void renderPoints();
    void renderLinestrings();
    void renderPolygons();
    BoundingBox calculateBoundingBox() override;

    std::string getName() override; // Méthode virtuelle pour accéder au nom
    void setIsVisible(bool isVisible) override; // (Optionnel) Méthode virtuelle pour définir le nom
    BoundingBox getBoundingBox(BoundingBox boundingBox) override;


private:
    BoundingBox boundingBox;
    bool isVisible = true;
    std::string name = "no name";

    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::tuple<float, float, float>>>> polygons;
    std::vector<std::vector<std::vector<std::pair<float, float>>> > polygons2d;
};

#endif // LAYER2D_H
