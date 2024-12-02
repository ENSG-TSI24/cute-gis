#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H

#include "DataManagement.h"

class AbstractData : public DataManagement
{
public:
    AbstractData();
    AbstractData(const char* Path);
    ~AbstractData();
    // TODO LATER AbstractData(std::string Path, std::string CRS);
    const char* GetPath() {return this->FilePath;}
    GDALDriver* GetDriver() {return this->data->GetDriver();}
    GDALDataset* GetDATA() {return this->data;}
protected :
    GDALDataset* data;
//    std::string FilePath;
    const char* FilePath;
    // TODO LATER std::string CRS;
    void closeDataSet() ;
};

#endif // ABSTRACTDATA_H
