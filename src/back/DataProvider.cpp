#include "DataProvider.h"
#include <gdal_priv.h>
#include <ogrsf_frmts.h>
#include <iostream>
#include <string>
#include <algorithm>



DataProvider::DataProvider() : url(nullptr),m_dataset(nullptr),m_metadata(nullptr)
{
}
GDALDataset* DataProvider::getDataset() {
    return m_dataset;
}

char** DataProvider::displayMetadata() {
    // Check if the dataset is empty
    if (isEmpty()) {
        std::cout << "Dataset is empty or not loaded." << std::endl;
        return nullptr;
    }

    // Metadata keys for WMS and WMTS
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

    // If no WMS/WMTS metadata is found, check for WFS layers
    if (!metadataFound) {
        std::cout << "No WMS/WMTS metadata found. Checking for WFS layers..." << std::endl;

        int layerCount = m_dataset->GetLayerCount();
        if (layerCount == 0) {
            std::cout << "No layers found in the dataset." << std::endl;
            return nullptr;
        }

        // Allocate memory for the array of layer names
        char** liste_layers = new char*[layerCount + 1]; // +1 for the null terminator
        for (int i = 0; i < layerCount; ++i) {
            OGRLayer* layer = m_dataset->GetLayer(i);
            if (layer != nullptr) {
                const char* layerName = layer->GetName();
                liste_layers[i] = new char[strlen(layerName) + 1];
                strcpy(liste_layers[i], layerName);

            } else {
                std::cout << "  Layer " << i + 1 << ": [Error accessing layer]" << std::endl;
                liste_layers[i] = nullptr;
            }
        }

        // Add null terminator to the array
        liste_layers[layerCount] = nullptr;

        return liste_layers;
    }

    return nullptr;
}

bool DataProvider::isEmpty(){
    return m_dataset==nullptr;
}

std::string DataProvider::GetTypeStream(const char* url){
       std::string urlLower = url;

       std::transform(urlLower.begin(), urlLower.end(), urlLower.begin(), ::tolower);


       // Check for specific keywords in the URL

       if (urlLower.find("wfs") != std::string::npos) {

           return "WFS";

       } else if (urlLower.find("wms") != std::string::npos) {

           return "WMS";

       } else if (urlLower.find("wmts") != std::string::npos) {

           return "WMTS";

       }  else {

           return "Unknown";

       }
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




