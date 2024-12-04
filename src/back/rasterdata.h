#ifndef RASTERDATA_H
#define RASTERDATA_H

#include "abstractdata.h"

class RasterData : public AbstractData
{
public:
    RasterData();
    RasterData(const char* path);
    ~RasterData();

};

#endif // RASTERDATA_H
