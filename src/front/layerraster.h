#ifndef LAYERTIFF_H
#define LAYERTIFF_H

#include <QGraphicsView>
#include "../back/rasterdata.h"
#include "boundingbox.h"

class LayerRaster
{
public:
    LayerRaster(RasterData raster);
    ~LayerRaster();
    void renderRasters();
    BoundingBox calculateBoundingBox();

    std::string name = "no name";
    QImage image;
    std::pair<double, double> topLeft;
    std::pair<double, double> bottomRight;
    BoundingBox boundingBox;
};

#endif // LAYERTIFF_H
