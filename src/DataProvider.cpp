#include "DataProvider.h"
#include <gdal_priv.h>
#include <ogrsf_frmts.h>
#include <iostream>



DataProvider::DataProvider() : url(nullptr),m_dataset(nullptr),m_metadata(nullptr)
{
}
GDALDataset* DataProvider::GetDataset() {
    return m_dataset;
}

// Get Metadata
void DataProvider::displayMetadata() {
    if (!isEmpty()){
        m_metadata = m_dataset->GetMetadata();

    if (m_metadata == nullptr) {
        std::cout << "no metadata for this WMS dataset" <<std::endl;

    } else {
        // ** change it later to be displayed in a Qt textbox
       std::cout << "WMS MetaData: " << std::endl;
        for (int i = 0; m_metadata[i] != nullptr; ++i) {
            std::cout << m_metadata[i] << std::endl;
        }
    }

    }
}

bool DataProvider::isEmpty(){
    return m_dataset==nullptr;
}

// Crucial to free memory, otherwise bugs
DataProvider::~DataProvider() {
    // Si le dataset existe, on le ferme
    if (m_dataset != nullptr) {
        //m_dataset->FlushCache();  // Facultatif : vide le cache du dataset avant de le fermer
        GDALClose(m_dataset);     // Libère la mémoire associée au dataset
        //m_dataset = nullptr;      // Nullifie le pointeur pour éviter les références ultérieures
    }
}




