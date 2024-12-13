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

std::string DataProvider::extractLayerName(const std::string& metadataLine) {
    size_t startPos = metadataLine.find("LAYERS=") + 7; // Position after "LAYERS="
    size_t endPos = metadataLine.find("&CRS", startPos); // Find the next "&CRS"
    if (startPos != std::string::npos && endPos != std::string::npos) {
        return metadataLine.substr(startPos, endPos - startPos);
    }
    return ""; // Return empty if not found
}

char** DataProvider::displayMetadata() {
    if (isEmpty()) {
           std::cout << "Dataset is empty or not loaded." << std::endl;
           return nullptr;
       }

       // Metadata keys for WMS and WMTS
       const char* keys[] = {"SUBDATASETS", "LAYERS", nullptr};
       std::vector<std::string> layerNames; // List to store layer names

       // Process metadata for WMS/WMTS
       for (int k = 0; keys[k] != nullptr; ++k) {
           m_metadata = m_dataset->GetMetadata(keys[k]);
           if (m_metadata != nullptr) {
               std::cout << "Metadata group: " << keys[k] << std::endl;

               for (int i = 0; m_metadata[i] != nullptr; ++i) {
                   std::string metadataLine = m_metadata[i];
                   std::cout << "  Metadata line: " << metadataLine << std::endl;

                   // Check for SUBDATASET_*_NAME for WMS
                   if (metadataLine.find("SUBDATASET_") == 0 && metadataLine.find("_NAME=") != std::string::npos) {
                       size_t pos = metadataLine.find("LAYERS=");
                       if (pos != std::string::npos) {
                           // Extract the layer name after "LAYERS="
                           std::string layerName = extractLayerName(metadataLine);
                           layerNames.push_back(layerName);
                           std::cout << "    Extracted layer name: " << layerName << std::endl;
                       }
                   } else if (metadataLine.find("layer=") != std::string::npos) {
                       // Handle standard WMTS metadata with "layer="
                       size_t pos = metadataLine.find("layer=");
                       std::string layerName = metadataLine.substr(pos + 6); // Skip "layer="
                       layerNames.push_back(layerName);
                       std::cout << "    Extracted layer name: " << layerName << std::endl;
                   }
               }
           }
       }

       // If no WMS/WMTS metadata was found, check for WFS layers
       if (layerNames.empty()) {
           std::cout << "No WMS/WMTS metadata found. Checking for WFS layers..." << std::endl;

           int layerCount = m_dataset->GetLayerCount();
           if (layerCount == 0) {
               std::cout << "No layers found in the dataset." << std::endl;
               return nullptr;
           }

           // Traverse WFS layers
           for (int i = 0; i < layerCount; ++i) {
               OGRLayer* layer = m_dataset->GetLayer(i);
               if (layer != nullptr) {
                   const char* layerName = layer->GetName();
                   layerNames.push_back(layerName); // Add to the list
                   std::cout << "  Layer name: " << layerName << std::endl;
               } else {
                   std::cout << "  Layer " << i + 1 << ": [Error accessing layer]" << std::endl;
               }
           }
       }

       // Check if any layers were found, otherwise return nullptr
       if (layerNames.empty()) {
           return nullptr;
       }

       // Allocate memory for the char** array of layer names
       char** liste_layers = new char*[layerNames.size() + 1]; // +1 for null terminator

       // Copy layer names into the liste_layers array
       for (size_t i = 0; i < layerNames.size(); ++i) {
           liste_layers[i] = new char[layerNames[i].size() + 1];
           strcpy(liste_layers[i], layerNames[i].c_str());
       }

       // Add a null terminator to the end of the array
       liste_layers[layerNames.size()] = nullptr;

       return liste_layers;
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




