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

    std::pair<double, double> GetGeoCoordinates(int pixelX, int pixelY);
    std::vector<std::pair<double, double>> GetGeoCoordinatesForPixels(const std::vector<std::pair<int, int>>& pixelCoordinates);
    QImage *GetImage();

    int GetWidth() const;
    int GetHeight() const;

protected:
    const char* filePath;
    int width;
    int height;
    QImage* image;

};

#endif // RASTERDATA_H
