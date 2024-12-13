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
    void render() override;
    BoundingBox calculateBoundingBox() override;

    std::string getName() override;
    void setName(std::string name) override;
    bool getIsVisible() override;
    void setIsVisible(bool isVisible) override;
    BoundingBox getBoundingBox() override;

private:
    BoundingBox boundingBox;
    bool isVisible = true;
    std::string name = "no name";

    QImage image;
    std::pair<double, double> topLeft;
    std::pair<double, double> bottomRight;
};

#endif // LAYERTIFF_H
