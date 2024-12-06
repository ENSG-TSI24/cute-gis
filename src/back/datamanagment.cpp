#include "datamanagment.h"
#include <iostream>
#include <gdal.h>
#include <gdal_priv.h>
#include <ogrsf_frmts.h>

DataManagment::DataManagment() {
    // data = nullptr;
    filePath = nullptr;
}

DataManagment::DataManagment(const char* Path) {
    GDALAllRegister();
    // data = (GDALDataset *) GDALOpenEx(Path, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    //  std::cout << "from cosntructor : " << data << std::endl;
    filePath = Path;
}

DataManagment::~DataManagment() {
    // GDALClose(data);
}

const char* DataManagment::GetPath() {
    return filePath;
}
/*
GDALDriver* DataManagment::GetDriver() {
    return data->GetDriver();
}

GDALDataset* DataManagment::GetDATA() {
    return data;
}
*/
