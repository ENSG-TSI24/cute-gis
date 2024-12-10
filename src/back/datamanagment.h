#ifndef DATAMANAGMENT_H
#define DATAMANAGMENT_H

#include <gdal.h>
#include <gdal_priv.h>


class DataManagment {
public:
    DataManagment();
    DataManagment(const char* Path);
    ~DataManagment();
    const char* GetPath();
protected:
    const char* filePath;
};

#endif // DATAMANAGMENT_H
