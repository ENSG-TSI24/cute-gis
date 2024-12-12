#ifndef LAYERTIFF_H
#define LAYERTIFF_H

#include <QGraphicsView>
#include "../back/rasterdata.h"
#include "boundingbox.h"
#include "layerbase.h"

class LayerRaster : public LayerBase
{
public:
    explicit LayerRaster(RasterData raster);
    ~LayerRaster() override;
    void renderRasters();
    BoundingBox calculateBoundingBox() override;

    std::string getName() override; // Méthode virtuelle pour accéder au nom
    void setIsVisible(bool isVisible) override; // (Optionnel) Méthode virtuelle pour définir le nom
    BoundingBox getBoundingBox(BoundingBox boundingBox) override;

private:
    BoundingBox boundingBox;
    bool isVisible = true;
    std::string name = "no name";

    QImage image;
    std::pair<double, double> topLeft;
    std::pair<double, double> bottomRight;
};

#endif // LAYERTIFF_H
