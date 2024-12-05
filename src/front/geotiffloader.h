#ifndef GEOTIFFLOADER_H
#define GEOTIFFLOADER_H

#include <gdal_priv.h>
#include <QImage>
#include <QString>

class GeoTiffLoader
{
public:
    static QImage loadGeoTIFF(const QString& filePath);
};

#endif // GEOTIFFLOADER_H
