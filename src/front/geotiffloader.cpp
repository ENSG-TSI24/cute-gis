#include "geotiffloader.h"
#include <stdexcept>
#include <vector>
#include <iostream>

GeoTiffLoader::GeoTiffLoader() {}

void GeoTiffLoader::loadGeoTIFF(const QString& filePath) {
    GDALAllRegister();
    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(filePath.toStdString().c_str(), GA_ReadOnly));
    if (!dataset) {
        throw std::runtime_error("Failed to open GeoTIFF file.");
    }

    GDALRasterBand* band = dataset->GetRasterBand(1);
    if (!band) {
        GDALClose(dataset);
        throw std::runtime_error("Failed to read raster band.");
    }

    int width = band->GetXSize();
    int height = band->GetYSize();
    std::vector<uint8_t> buffer(width * height);

    band->RasterIO(GF_Read, 0, 0, width, height, buffer.data(), width, height, GDT_Byte, 0, 0);

    // Création et remplissage de l'image
    QImage* _image = new QImage(width, height, QImage::Format_Grayscale8);
    for (int y = 0; y < height; ++y) {
        memcpy(_image->scanLine(y), buffer.data() + y * width, width);
    }

    std::cout << "Image créée, format : " << _image->format() << std::endl;

    this->image = _image;
    std::cout << "Image attribuée, format : " << this->image->format() << std::endl;

    GDALClose(dataset);
}
