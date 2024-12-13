#include "rasterdata.h"
#include <iostream>

RasterData::RasterData() : filePath(nullptr), image(nullptr)
{
}

RasterData::RasterData(const char* path) : filePath(path), width(0), height(0), image(nullptr) {
    GDALAllRegister();
    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(path, GA_ReadOnly));
    if (dataset) {
        width = dataset->GetRasterXSize();
        height = dataset->GetRasterYSize();
        GDALClose(dataset);
    } else {
        std::cerr << "Failed to open raster file: " << path << std::endl;
    }
}

RasterData::~RasterData(){}

int RasterData::GetWidth() const {
    return width;
}

int RasterData::GetHeight() const {
    return height;
}

std::pair<double, double> RasterData::GetTopLeftGeoCoordinates() const {
    std::pair<double, double> topLeftCoords = this->GetGeoCoordinates(0, 0);
    std::cout << "Top Left GeoCoordinates: (" << topLeftCoords.first << ", " << topLeftCoords.second << ")" << std::endl;
    return topLeftCoords;  // Le coin supérieur gauche est (0, 0)
}

std::pair<double, double> RasterData::GetBottomRightGeoCoordinates() const {
    std::pair<double, double> bottomRightCoords = this->GetGeoCoordinates(this->GetWidth() - 1, this->GetHeight() - 1);
    std::cout << "Bottom Right GeoCoordinates: (" << bottomRightCoords.first << ", " << bottomRightCoords.second << ")" << std::endl;
    return bottomRightCoords;  // Le coin inférieur droit
}

// Get the coordinates of a single pixels
std::pair<double, double> RasterData::GetGeoCoordinates(int pixelX, int pixelY) const
{
    if (!filePath)
    {
        std::cerr << "No file path provided." << std::endl;
        return {0.0, 0.0};
    }

    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset*)GDALOpen(filePath, GA_ReadOnly);
    if (!dataset)
    {
        std::cerr << "Failed to open raster file." << std::endl;
        return {0.0, 0.0};
    }

    //////// GET THE AFFINE TRANSFORMATION /////////
    double geoTransform[6];
    if (dataset->GetGeoTransform(geoTransform) != CE_None)
    {
        std::cerr << "Failed to get geotransform." << std::endl;
        GDALClose(dataset);
        return {0.0, 0.0};
    }

    /////// CONVERT COORDINATE OF PIXELS ////////
    double xGeo = geoTransform[0] + geoTransform[1] * pixelX + geoTransform[2] * pixelY;
    double yGeo = geoTransform[3] + geoTransform[4] * pixelX + geoTransform[5] * pixelY;

    GDALClose(dataset);

    return {xGeo, yGeo};
}

// This function get coordinnates for any number pixels
std::vector<std::pair<double, double>> RasterData::GetGeoCoordinatesForPixels(const std::vector<std::pair<int, int>>& pixelCoordinates)
{
    std::vector<std::pair<double, double>> geoCoordinates;

    if (!filePath)
    {
        std::cerr << "No file path provided." << std::endl;
        return geoCoordinates;
    }

    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset*)GDALOpen(filePath, GA_ReadOnly);
    if (!dataset)
    {
        std::cerr << "Failed to open raster file." << std::endl;
        return geoCoordinates;
    }

    ////// AFFINE TRANSFORMAITON ///////
    double geoTransform[6];
    if (dataset->GetGeoTransform(geoTransform) != CE_None)
    {
        std::cerr << "Failed to get geotransform." << std::endl;
        GDALClose(dataset);
        return geoCoordinates;
    }

    /////// CALCULATE TRANSORMATION  FOR EACH PIXELS ///////
    for (const auto& coord : pixelCoordinates)
    {
        int pixelX = coord.first;
        int pixelY = coord.second;

        double xGeo = geoTransform[0] + geoTransform[1] * pixelX + geoTransform[2] * pixelY;
        double yGeo = geoTransform[3] + geoTransform[4] * pixelX + geoTransform[5] * pixelY;

        geoCoordinates.push_back({xGeo, yGeo});
    }

    GDALClose(dataset);
    return geoCoordinates;
}


QImage RasterData::GetImage() {
    if (!filePath) {
        std::cerr << "No file path provided." << std::endl;
    }

    GDALAllRegister();
    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(filePath, GA_ReadOnly));
    if (!dataset) {
        std::cerr << "Failed to open raster file." << std::endl;
    }

    int width = dataset->GetRasterXSize();
    int height = dataset->GetRasterYSize();
    int bands = dataset->GetRasterCount();

    if (bands < 3) { // We need at least 3 bands for the raster in color
        std::cerr << "Not enough raster bands (expected 3 for RGB)." << std::endl;
        GDALClose(dataset);
    }

    // Buffer for 3 bands
    std::vector<unsigned char> buffer(width * height * 3);

    // Reading the 3 bands Red, Blue, Green with RasterIO
    if (dataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, width, height, buffer.data(), width, height, GDT_Byte, 0, 0) != CE_None ||
        dataset->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, width, height, buffer.data() + width * height, width, height, GDT_Byte, 0, 0) != CE_None ||
        dataset->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, width, height, buffer.data() + 2 * width * height, width, height, GDT_Byte, 0, 0) != CE_None) {
        std::cerr << "Failed to read RGB bands." << std::endl;
        GDALClose(dataset);
    }

    const size_t maxLinesToDisplay = 3; // Limite du nombre de lignes à afficher

    //////////COUT TO CHECK THE BUFFER DATA //////////////
    /*
    std::cout << "Red Band (limited display):" << std::endl;
    for (size_t i = 0; i < width * height; ++i) {
        if (i / width >= maxLinesToDisplay) break; // Arrêter après 'maxLinesToDisplay' lignes
        if (i > 0 && i % width == 0) std::cout << std::endl; // Saut de ligne après chaque ligne
        std::cout << static_cast<int>(buffer[i]) << " ";
    }
    std::cout << std::endl;

    std::cout << "Green Band (limited display):" << std::endl;
    for (size_t i = 0; i < width * height; ++i) {
        if (i / width >= maxLinesToDisplay) break; // Arrêter après 'maxLinesToDisplay' lignes
        if (i > 0 && i % width == 0) std::cout << std::endl;
        std::cout << static_cast<int>(buffer[i + width * height]) << " ";
    }
    std::cout << std::endl;

    std::cout << "Blue Band (limited display):" << std::endl;
    for (size_t i = 0; i < width * height; ++i) {
        if (i / width >= maxLinesToDisplay) break; // Arrêter après 'maxLinesToDisplay' lignes
        if (i > 0 && i % width == 0) std::cout << std::endl;
        std::cout << static_cast<int>(buffer[i + 2 * width * height]) << " ";
    }
    std::cout << std::endl;
    */

    // QImage Building
    auto qImage = std::make_unique<QImage>(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            int r = buffer[index];
            int g = buffer[index + width * height];
            int b = buffer[index + 2 * width * height];
            qImage->setPixel(x, y, qRgb(r, g, b));
        }
    }


    GDALClose(dataset);

    return *qImage.release();
}

void RasterData::displayData()
{
    // Afficher les informations principales
    std::cout << "Raster Width: " << this->GetWidth() << std::endl;
    std::cout << "Raster Height: " << this->GetHeight() << std::endl;

    // Afficher les coordonnées géographiques de quelques pixels
    std::cout << "Geo-coordinates for pixel (0, 0):" << std::endl;
    auto geoCoords = this->GetGeoCoordinates(0, 0);
    std::cout << "Lon: " << geoCoords.first << ", Lat: " << geoCoords.second << std::endl;

    std::cout << "Geo-coordinates for pixel (width / 2, height / 2):" << std::endl;
    geoCoords = this->GetGeoCoordinates(this->GetWidth() / 2, this->GetHeight() / 2);
    std::cout << "Lon: " << geoCoords.first << ", Lat: " << geoCoords.second << std::endl;

    // Tester des coordonnées pour plusieurs pixels
    std::vector<std::pair<int, int>> pixelCoordinates = {
        {0, 0},
        {this->GetWidth() / 2, this->GetHeight() / 2},
        {this->GetWidth() - 1, this->GetHeight() - 1}
    };
    auto geoCoordsBatch = this->GetGeoCoordinatesForPixels(pixelCoordinates);
    std::cout << "Geo-coordinates for batch of pixels:" << std::endl;
    for (size_t i = 0; i < geoCoordsBatch.size(); ++i) {
        std::cout << "Pixel (" << pixelCoordinates[i].first << ", " << pixelCoordinates[i].second
                  << "): Lon: " << geoCoordsBatch[i].first
                  << ", Lat: " << geoCoordsBatch[i].second << std::endl;
    }

    auto geoCoordsLowerRight = this->GetGeoCoordinates(this->GetWidth() - 1, this->GetHeight() - 1);
    std::cout << "Geo-coordinates for the lower-right corner:" << std::endl;
    std::cout << "Lon: " << geoCoordsLowerRight.first << ", Lat: " << geoCoordsLowerRight.second << std::endl;

}
