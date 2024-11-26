#ifndef WMSCLASS_H
#define WMSCLASS_H

#include <gdal_priv.h>
#include <cpl_conv.h>
#include <string>
#include <iostream>
#include <fstream>

class WMSClass
{
private:

    const char* wms_url;
    //std::string output_file;
    GDALDataset* wms_dataset; 
    char** wms_metada; 
    

public:

    WMSClass(const char* url);
    ~WMSClass();
    void loadDataset();
    GDALDataset* getDataset();
    void displayMetadata();
    //std::string exportToGeotiff (int n, const std::string& outputFilePath);


};

#endif // WMSCLASS_H