#include "DataProvider.h"
#include <gdal_priv.h>
#include <ogrsf_frmts.h>
#include <iostream>



DataProvider::DataProvider() : url(nullptr),m_dataset(nullptr),m_metadata(nullptr)
{
}
GDALDataset* DataProvider::getDataset() {
    return m_dataset;
}

// Get Metadata
void DataProvider::displayMetadata() {
    if (isEmpty()) {
            std::cout << "Dataset is empty or not loaded." << std::endl;
            return;
        }
    else {
        m_metadata = m_dataset->GetMetadata("SUBDATASETS");
    // ** change it later to be displayed in a Qt textbox


    if (m_metadata == nullptr) {
        std::cout << "no metadata for this WMS dataset" <<std::endl;

    } else {
        const char* keys[] = {"SUBDATASETS", "LAYERS", nullptr}; // Essayez d'autres clés ici
        for (int k = 0; keys[k] != nullptr; ++k) {
            std::cout << "Checking metadata group: " << keys[k] << std::endl;

            m_metadata = m_dataset->GetMetadata(keys[k]);
            if (m_metadata != nullptr) {
                for (int i = 0; m_metadata[i] != nullptr; ++i) {
                    std::cout << m_metadata[i] << std::endl;
                }

    }
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
        m_dataset->FlushCache();  // Facultatif : vide le cache du dataset avant de le fermer
        GDALClose(m_dataset);     // Libère la mémoire associée au dataset
        m_dataset = nullptr;      // Nullifie le pointeur pour éviter les références ultérieures
    }
}




