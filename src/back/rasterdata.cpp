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
    if(image)
    {
        delete image;
    }
}

QImage* RasterData::GetImage()
{
    if(!filePath)
    {
        std::cerr << "No file path provided." << std::endl;
        return nullptr;
    }

    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset*)GDALOpen(filePath, GA_ReadOnly);
    if(!dataset)
    {
        std::cerr << "Failed to open raster file." << std::endl;
        return nullptr;
    }

    int width = dataset->GetRasterXSize();
    int height = dataset->GetRasterYSize();
    int bands = dataset->GetRasterCount();

    if (bands < 1)
    {
        std::cerr << "No raster bands found." << std::endl;
        GDALClose(dataset);
        return nullptr;
    }

    GDALRasterBand* band = dataset->GetRasterBand(1);
    if(!band)
    {
        std::cerr << "Failed to get raster band." << std::endl;
        GDALClose(dataset);
        return nullptr;
    }

    std::vector<unsigned char> buffer(width * height);
    if (band->RasterIO(GF_Read, 0, 0, width, height, buffer.data(), width, height, GDT_Byte, 0, 0) != CE_None)
    {
        std::cerr << "Failed to read data." << std::endl;
        GDALClose(dataset);
        return nullptr;
    }

    image = new QImage(buffer.data(), width, height, QImage::Format_Grayscale8);

    GDALClose(dataset);

    return image;
}
