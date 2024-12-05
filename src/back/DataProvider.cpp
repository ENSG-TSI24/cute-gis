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

void DataProvider::displayMetadata() {
    if (isEmpty()) {
        std::cout << "Dataset is empty or not loaded." << std::endl;
        return;
    }

    // Cas général pour WMS et WMTS (métadonnées classiques)
    const char* keys[] = {"SUBDATASETS", "LAYERS", nullptr};
    bool metadataFound = false;

    for (int k = 0; keys[k] != nullptr; ++k) {
        m_metadata = m_dataset->GetMetadata(keys[k]);
        if (m_metadata != nullptr) {
            metadataFound = true;
            std::cout << "Metadata group: " << keys[k] << std::endl;
            for (int i = 0; m_metadata[i] != nullptr; ++i) {
                std::cout << "  " << m_metadata[i] << std::endl;
            }
        }
    }

    // Si aucune métadonnée n'a été trouvée pour WMS/WMTS, essayons pour WFS
    if (!metadataFound) {
        std::cout << "No WMS/WMTS metadata found. Checking for WFS layers..." << std::endl;

        int layerCount = m_dataset->GetLayerCount();
        if (layerCount == 0) {
            std::cout << "No layers found in the dataset." << std::endl;
            return;
        }

        std::cout << "Number of layers (FeatureTypes): " << layerCount << std::endl;
        for (int i = 0; i < layerCount; ++i) {
            OGRLayer* layer = m_dataset->GetLayer(i);
            if (layer != nullptr) {
                std::cout << "  Layer " << i + 1 << ": " << layer->GetName() << std::endl;
            } else {
                std::cout << "  Layer " << i + 1 << ": [Error accessing layer]" << std::endl;
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




