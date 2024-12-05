#include "datamanagment.h"

#include <gdal.h>
#include <gdal_priv.h>

DataManagment::DataManagment() {
    data = nullptr;
    filePath = nullptr;
}

DataManagment::DataManagment(const char* Path) {
    data = (GDALDataset *) GDALOpenEx(Path, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    filePath = Path;
}

DataManagment::~DataManagment() {
    closeDataSet();
}

DataManagment::closeDataSet() {
    GDALClose(data);
}

const char* DataManagment::GetPath() {
    return filePath;
}

GDALDriver* DataManagment::GetDriver() {
    return data->GetDriver();
}

GDALDataset* DataManagment::GetDATA() {
    return data;
}