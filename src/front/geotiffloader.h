#ifndef GEOTIFFLOADER_H
#define GEOTIFFLOADER_H

#include <gdal_priv.h>
#include <QImage>
#include <QString>

class GeoTiffLoader
{
public:
    GeoTiffLoader();
    void loadGeoTIFF(const QString& filePath);
    QImage* image;
};

#endif // GEOTIFFLOADER_H
