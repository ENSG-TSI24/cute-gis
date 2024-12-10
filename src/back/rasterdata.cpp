#include "rasterdata.h"
#include <iostream>

RasterData::RasterData() : filePath(nullptr), image(nullptr)
{
}

RasterData::RasterData(const char* path) : filePath(path), image(nullptr)
{
}

RasterData::~RasterData()
{
    if (image)
    {
        delete image;
    }
}

int RasterData::GetWidth() const {
    return width;
}

int RasterData::GetHeight() const {
    return height;
}

// Get the coordinates of a single pixels
std::pair<double, double> RasterData::GetGeoCoordinates(int pixelX, int pixelY)
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


QImage* RasterData::GetImage()
{
    if (!filePath)
    {
        std::cerr << "No file path provided." << std::endl;
        return nullptr;
    }

    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset*)GDALOpen(filePath, GA_ReadOnly);
    if (!dataset)
    {
        std::cerr << "Failed to open raster file." << std::endl;
        return nullptr;
    }

    /////// SIZE IMAGE INITIALIZATION //////
    width = dataset->GetRasterXSize();
    height = dataset->GetRasterYSize();
    int bands = dataset->GetRasterCount();

    if (bands < 1)
    {
        std::cerr << "No raster bands found." << std::endl;
        GDALClose(dataset);
        return nullptr;
    }

    double geoTransform[6];
    if (dataset->GetGeoTransform(geoTransform) != CE_None)
    {
        std::cerr << "Failed to get geotransform." << std::endl;
        GDALClose(dataset);
        return nullptr;
    }

    std::vector<unsigned char> buffer(width * height * 3);

    GDALRasterBand* rBand = dataset->GetRasterBand(1);
    GDALRasterBand* gBand = dataset->GetRasterBand(2);
    GDALRasterBand* bBand = dataset->GetRasterBand(3);

    if (rBand->RasterIO(GF_Read, 0, 0, width, height, buffer.data(), width, height, GDT_Byte, 0, 0) != CE_None ||
        gBand->RasterIO(GF_Read, 0, 0, width, height, buffer.data() + width * height, width, height, GDT_Byte, 0, 0) != CE_None ||
        bBand->RasterIO(GF_Read, 0, 0, width, height, buffer.data() + 2 * width * height, width, height, GDT_Byte, 0, 0) != CE_None)
    {
        std::cerr << "Failed to read RGB bands." << std::endl;
        GDALClose(dataset);
        return nullptr;
    }

    if (image)
    {
        delete image;
    }

    image = new QImage(buffer.data(), width, height, QImage::Format_RGB888);

    const char* projection = dataset->GetProjectionRef();
    if (projection)
    {
        std::cout << "Projection: " << projection << std::endl;
    }
    else
    {
        std::cout << "No projection information available." << std::endl;
    }

    ////// SIZE OF PIXELS IN METERS ///////
    double pixelSizeX = geoTransform[1];
    double pixelSizeY = geoTransform[5];

    std::cout << "Pixel size in X: " << pixelSizeX << " meters" << std::endl;
    std::cout << "Pixel size in Y: " << pixelSizeY << " meters" << std::endl;

    GDALClose(dataset);
    return image;
}





