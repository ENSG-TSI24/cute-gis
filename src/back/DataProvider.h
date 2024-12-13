#pragma once
#include <gdal_priv.h>
#include <ogrsf_frmts.h>
#include <iostream>

class DataProvider {
    public:
        DataProvider() ;
        GDALDataset* getDataset() ;

        // Get Metadata
        char** displayMetadata();

        bool isEmpty() ;
        std::string GetTypeStream(const char* url);
        // Crucial to free memory, otherwise bugs
        ~DataProvider() ;

    protected:
        const char* url;
        GDALDataset* m_dataset;
        char** m_metadata;
        std::string extractLayerName(const std::string& metadataLine);

};
