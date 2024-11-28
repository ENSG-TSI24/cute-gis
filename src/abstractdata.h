#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H

#include "DataManagement.h"

class AbstractData : public DataManagement
{
public:
    AbstractData();
    AbstractData(char* Path);
    ~AbstractData();
    // TODO LATER AbstractData(std::string Path, std::string CRS);
protected :
    GDALDataset* data;
//    std::string FilePath;
    char* FilePath;
    // TODO LATER std::string CRS;
    std::string GetPath() {return this->FilePath;}
    GDALDriver* GetDriver() {return this->data->GetDriver();}
    GDALDataset* GetDATA() {return this->data;}
    void closeDataSet() ;
};

#endif // ABSTRACTDATA_H
