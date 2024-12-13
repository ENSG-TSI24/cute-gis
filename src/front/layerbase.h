#ifndef LAYERBASE_H
#define LAYERBASE_H

#include <iostream>
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
};


#endif // LAYERBASE_H
