#ifndef RASTERDATA_H
#define RASTERDATA_H

#include "datamanagment.h"
#include <QImage>

class RasterData : public DataManagment
{
public:
    RasterData();
    RasterData(const char* path);
    ~RasterData();

    QImage *GetImage();

protected:
    const char* filePath;
    QImage* image;

};

#endif // RASTERDATA_H
