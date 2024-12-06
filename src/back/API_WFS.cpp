#include "API_WFS.h"
#include <iostream>
#include <ogrsf_frmts.h>
#include <fstream>

#include <iostream>
#include <filesystem>
#include <string>
#include "gdal_priv.h"
#include "gdal_priv.h"


API_WFS::API_WFS(const char* link) : url(link)
{
    GDALAllRegister();
}


void API_WFS::loadDataset() {
    // Ouvrir le dataset avec GDAL pour un flux WMS
    m_dataset = static_cast<GDALDataset*>(
        GDALOpenEx(url,GDAL_OF_VECTOR, nullptr, nullptr, nullptr));

    /* Display error message and exit program if dataset fails to open correctly ** to be replaced
        when the front end team finishes ( to reinsert or close window) */
    if (isEmpty()) {
        std::cerr << "Error: Impossible to connect to WFS or unsupported format" << std::endl;
        throw std::runtime_error("Failed to load dataset"); // Lancer une exception au lieu d'exit
    }
}


void API_WFS::getData(const char* name)
{

    std::cout << "oui";
    if (isEmpty())
    {
        std::cout << "Dataset is not opened." << std::endl;
        return;
    }


    // Log the number of layers
        int layerCount = m_dataset->GetLayerCount();
        std::cout << layerCount;
       // std::cout << "THE DATASET CONTAINS THIS MANY LAYERS >>>>> " << layerCount << std::endl;

        OGRLayer* layer = nullptr;

        if (layerCount == 1 || std::string(name) == "")
        {
            // Automatically select the first layer if there's just one or if no name is specified
            layer = m_dataset->GetLayer(0);
            std::cout << "Only one layer present. Automatically selected layer: "
                      << (layer ? layer->GetName() : "Unknown") << std::endl;
        }
        else
        {
            // Access the layer by name
            layer = m_dataset->GetLayerByName(name);
            if (layer == nullptr)
            {
                std::cout << "Layer " << name << " not found." << std::endl;
                return;
            }
        }

        if (layer == nullptr)
        {
            std::cout << "No layer could be accessed." << std::endl;
            return;
        }

    // Reset reading and iterate through features
    layer->ResetReading();
    OGRFeature* feature = nullptr;

    while ((feature = layer->GetNextFeature()) != nullptr)
    {
       // Access feature definition
        OGRFeatureDefn* featureDefinition = layer->GetLayerDefn();

        for (int i = 0; i < featureDefinition->GetFieldCount(); i++)
        {
            // Access field definition and print field values
            OGRFieldDefn* fieldDefinition = featureDefinition->GetFieldDefn(i);

            switch (fieldDefinition->GetType())
            {
                case OFTInteger:
                    std::cout << feature->GetFieldAsInteger(i) << ",";
                    break;
                case OFTReal:
                    std::cout << feature->GetFieldAsDouble(i) << ",";
                    break;
                case OFTString:
                    std::cout << feature->GetFieldAsString(i) << ",";
                    break;
                default:
                    std::cout << "Unknown field type, ";
                    break;
            }
        }

        // Access geometry and check if it's a point
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (geometry != nullptr && wkbFlatten(geometry->getGeometryType()) == wkbPoint)
        {
            OGRPoint* point = dynamic_cast<OGRPoint*>(geometry);
            if (point != nullptr)
            {
                //std::cout << point->getX() << "," << point->getY() << std::endl;
            }
        }
        else
        {
            std::cout << "No Point Geometry" << std::endl;
        }

        // Destroy the feature
        OGRFeature::DestroyFeature(feature);
    }


}


OGRLayer* API_WFS::GetLayer(const char* name)
{
    if (isEmpty())
    {
        std::cout << "Dataset is not opened." << std::endl;
        return nullptr;
    }

    if (std::string(name).empty())
    {
        // Automatically select the only layer if there's just one
        if (m_dataset->GetLayerCount() == 1)
        {
            return m_dataset->GetLayer(0);
        }
        else
        {
            std::cout << "Multiple layers present. Specify a layer name." << std::endl;
            return nullptr;
        }
    }

    // Get the layer by name
    return m_dataset->GetLayerByName(name);
}


void API_WFS::ExportToGeoJSON(const std::string& layerName)
{
    // Définir le chemin relatif pour le dossier GeoJSON
    std::string relativePath = "../cute-gis/data/geojson/";
    std::string outputFileName = layerName + ".geojson";
    const std::string& outputPath = relativePath + outputFileName;

    std::cout<<"je suis dans le fonctiion :" << outputPath<<std::endl;

    // Créer le dossier si nécessaire
    std::filesystem::create_directories(relativePath);

    if (isEmpty())
    {
        std::cout << "Dataset is not opened." << std::endl;
        return;

    }

    // Obtenir le driver GeoJSON
    GDALDriver* geoJsonDriver = GetGDALDriverManager()->GetDriverByName("GeoJSON");
    if (geoJsonDriver == nullptr)
    {
        std::cout << "GeoJSON driver not available." << std::endl;
        return;
    }

    // Créer un dataset GeoJSON pour écrire le fichier
    char** options = nullptr;
    GDALDataset* geoJsonDataset = geoJsonDriver->Create(outputPath.c_str(), 0, 0, 0, GDT_Unknown, options);
    if (geoJsonDataset == nullptr)
    {
        std::cout << "Failed to create GeoJSON file: " << outputPath << std::endl;
        return;
    }

    // Obtenir la couche par son nom
    OGRLayer* layer = m_dataset->GetLayerByName(layerName.c_str());
    if (layer == nullptr)
    {
        std::cout << "Layer '" << layerName << "' not found in the dataset." << std::endl;
        GDALClose(geoJsonDataset);
        return;

    }

    // Copier la couche dans le fichier GeoJSON
    if (geoJsonDataset->CopyLayer(layer, layer->GetName()) == nullptr)
    {
        std::cout << "Failed to copy layer to GeoJSON file." << std::endl;
        GDALClose(geoJsonDataset);
        return;

    }

    // Fermer le dataset pour sauvegarder le fichier sur le disque
    GDALClose(geoJsonDataset);

    // Allouer une nouvelle mémoire pour stocker le chemin
    static std::string outputPathStatic = outputPath;  // Variable statique pour persister la valeur
    output_path = outputPathStatic.c_str();


    std::cout<<"je suis dans le fonctiion :" << output_path<<std::endl;
    // Afficher le chemin
    std::cout << "GeoJSON successfully created at: " << output_path << std::endl;
}

const char* API_WFS:: getOutput(){
    std::cout << " hello world : " << output_path << std::endl;
    return output_path;
}
