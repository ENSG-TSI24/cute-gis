#ifndef LAYERBASE_H
#define LAYERBASE_H

#include <iostream>
#include <vector>
#include "boundingbox.h"


class LayerBase {

public:
    virtual ~LayerBase() = 0;
    virtual BoundingBox calculateBoundingBox() = 0;
    virtual void render() = 0;

    virtual std::string getName() = 0;
    virtual void setName(std::string name) = 0;
    virtual bool getIsVisible() = 0;
    virtual void setIsVisible(bool isVisible) = 0;
    virtual BoundingBox getBoundingBox() = 0;
    virtual float getOpacity() = 0;
    virtual void setOpacity(float opacity) = 0;
    virtual std::vector<std::vector<std::string>> getAttributes() = 0;
    virtual std::vector<std::string> getAttributeHeaders() = 0;
    virtual void highlightGeometry(int rowIndex) = 0;
    virtual bool hasAttributes() const = 0;
};


#endif // LAYERBASE_H
