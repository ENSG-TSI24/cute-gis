#include "geotiffloader.h"
#include <stdexcept>
#include <vector>

QImage GeoTiffLoader::loadGeoTIFF(const QString& filePath) {
    GDALAllRegister(); // Enregistrer tous les drivers GDAL

    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(filePath.toStdString().c_str(), GA_ReadOnly));
    if (!dataset) {
        throw std::runtime_error("Failed to open GeoTIFF file.");
    }

    // Lire la première bande (canal de couleur)
    GDALRasterBand* band = dataset->GetRasterBand(1);
    if (!band) {
        GDALClose(dataset);
        throw std::runtime_error("Failed to read raster band.");
    }

    int width = band->GetXSize();
    int height = band->GetYSize();
    std::vector<uint8_t> buffer(width * height);

    // Lire les données dans un tampon
    band->RasterIO(GF_Read, 0, 0, width, height, buffer.data(), width, height, GDT_Byte, 0, 0);

    // Créer une QImage
    QImage image(buffer.data(), width, height, QImage::Format_Grayscale8);

    GDALClose(dataset);
    return image;
}
