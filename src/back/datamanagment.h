#ifndef DATAMANAGMENT_H
#define DATAMANAGMENT_H

#include <gdal.h>
#include <gdal_priv.h>


class DataManagment {
public:
    DataManagment();
    DataManagment(const char* Path);
    ~DataManagment();
    // TODO LATER DataManagment(std::string Path, std::string CRS);
    const char* GetPath();
    // GDALDriver* GetDriver();
    // GDALDataset* GetDATA();
protected:
    // GDALDataset* data;
    const char* filePath;
};

#endif // DATAMANAGMENT_H
