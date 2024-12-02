#include "WMSClass.h"

/* user will input an XML file that will be captured from file browser element 
that the frontend team will build ** to be added in the constructor */

WMSClass::WMSClass(const char* url) : wms_url(url) {
        GDALAllRegister();

}

void WMSClass::loadDataset(){

    wms_dataset = static_cast<GDALDataset*>(
    GDALOpenEx(wms_url,GDAL_OF_ALL, nullptr, nullptr, nullptr));

    /* Display error message and exit program if dataset fails to open correctly ** to be replaced 
    when the front end team finishes ( to reinsert or close window) */
    if (isEmpty())
    {
        std::cout << "Error: Impossible to connect to WMS or unsupported format" << std::endl;
        exit(1); // ** to be replaced later 
    }
}


GDALDataset* WMSClass::getDataset(){
    return m_dataset;
}

// Get Metadata 
void WMSClass::displayMetadata() {
    if (!isEmpty()){
        wms_metada = dataset->GetMetadata(); 

    if (wms_metada == nullptr) {
        std::cout << "no metadata for this WMS dataset" <<std::endl;
        
    } else {
        // ** change it later to be displayed in a Qt textbox
       std::cout << "WMS MetaData: " << std::endl;
        for (int i = 0; metadata[i] != nullptr; ++i) {
            std::cout << metadata[i] << std::endl;
        } 
    }

    }
}

bool WMSClass::isEmpty(){
    return wms_dataset==nullptr;
}

// Crucial to free memory, otherwise bugs 
WMSClass::~WMSClass() {
    GDALClose(wms_dataset);
}


