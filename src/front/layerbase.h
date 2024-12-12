#ifndef LAYERBASE_H
#define LAYERBASE_H

#include <iostream>
#include "boundingbox.h"


class LayerBase {

public:
    virtual ~LayerBase() = 0;
    virtual BoundingBox calculateBoundingBox() = 0;;

    virtual std::string getName() = 0; // Méthode virtuelle pour accéder au nom
    virtual void setIsVisible(bool isVisible) = 0; // (Optionnel) Méthode virtuelle pour définir le nom
    virtual BoundingBox getBoundingBox(BoundingBox boundingBox) = 0;
};


#endif // LAYERBASE_H
