
#include <gdal_priv.h>
#include <ogrsf_frmts.h>
#include <iostream>

class DataProvider {
    public:
        DataProvider() ;

        GDALDataset* GetDataset() ;

        // Get Metadata
        void displayMetadata() ;

        bool isEmpty();

        // Crucial to free memory, otherwise bugs
        ~DataProvider() ;



    protected:
        const char* url;
        GDALDataset* m_dataset;
        char** m_metadata;

};
