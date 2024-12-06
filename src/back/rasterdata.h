#ifndef RASTERDATA_H
#define RASTERDATA_H

#include "datamanagment.h"

class RasterData : public DataManagment
{
public:
    RasterData();
    RasterData(const char* path);
    ~RasterData();

};

#endif // RASTERDATA_H
