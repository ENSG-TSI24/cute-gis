#include "abstractdata.h"
#include <gdal.h>
#include <gdal_priv.h>

AbstractData::AbstractData() : data(nullptr), FilePath(nullptr) {}

AbstractData::AbstractData(char* path): data(nullptr), FilePath(nullptr)
{
    GDALAllRegister();
    this->FilePath = path;
    this->data = (GDALDataset *) GDALOpenEx(path, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
}

AbstractData::~AbstractData() {
    GDALClose(this->data);
}
