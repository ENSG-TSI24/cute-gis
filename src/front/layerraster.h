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
    void highlightGeometry(int rowIndex) override;

    std::string getName() override;
    void setName(std::string name) override;
    bool getIsVisible() override;
    void setIsVisible(bool isVisible) override;
    BoundingBox getBoundingBox() override;
    float getOpacity() override;
    void setOpacity(float opacity) override;
    std::vector<std::vector<std::string>> getAttributes() override;
    std::vector<std::string> getAttributeHeaders() override;

    bool hasAttributes() const override{
        return false;
    }


private:
    BoundingBox boundingBox;
    bool isVisible = true;
    std::string name = "no name";
    float opacity = 1.0f;
    std::vector<std::vector<std::string>> attributes;
    std::vector<std::string> attributeHeaders;
    int highlightedIndex = -1;

    QImage image;
    std::pair<double, double> topLeft;
    std::pair<double, double> bottomRight;
};

#endif // LAYERTIFF_H
