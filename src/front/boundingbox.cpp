#include "boundingbox.h"

BoundingBox::BoundingBox(){}

BoundingBox::BoundingBox(float _minX, float _maxX, float _minY, float _maxY)
    :minX(_minX), maxX(_maxX), minY(_minY), maxY(_maxY){}

BoundingBox::~BoundingBox(){}
