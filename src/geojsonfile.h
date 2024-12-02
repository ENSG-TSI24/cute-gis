#ifndef GEOJSONFILE_H
#define GEOJSONFILE_H

#include "vectordata.h"

class GeoJsonFile : public VectorData
{
public:
    GeoJsonFile();
    GeoJsonFile(const char* path);
    const char* GetCRS();
    char** GetMetadata();
    void PrintGeoJsonAttributes();
};

#endif // GEOJSONFILE_H
