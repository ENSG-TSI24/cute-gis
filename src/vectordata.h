#ifndef VECTORDATA_H
#define VECTORDATA_H

#include "abstractdata.h"

class VectorData : public AbstractData
{
public:
    VectorData();
    VectorData(char* path);
protected :
    GDALDriver *GeoJsonDriver;

};

#endif // VECTORDATA_H
