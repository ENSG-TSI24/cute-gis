#ifndef DATAMANAGMENT_H
#define DATAMANAGMENT_H

#include <gdal.h>
#include <gdal_priv.h>


class DataManagment {
public:
    DataManagment();
    /**
    * @brief this class is not to be instancied, this is an abstraction of RasterData and VectorData.
    * @param const char* Path : path to the file, can be an url also
    */
    DataManagment(const char* Path);
    ~DataManagment();
    /**
    * @brief return path of the file associated to the DataManagement class.
    * @param const char* Path : path to the file, can be an url also
    */
    const char* GetPath();
protected:
    const char* filePath;
};

#endif // DATAMANAGMENT_H
