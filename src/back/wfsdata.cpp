#include "wfsdata.h"
#include <iostream>
#include <ogrsf_frmts.h>
#include <fstream>

WFSData::WFSData() : m_dataset(nullptr) {
    
}

WFSData::~WFSData()
{
    if (m_dataset != nullptr)
    {
        GDALClose(m_dataset);
    }
}

GDALDataset* WFSData::GetDataset(){
    return m_dataset;
}


void WFSData::open(const char* link)
{
    GDALAllRegister();
    // Open the dataset using the GDALDataset class
    m_dataset = static_cast<GDALDataset*>(
        GDALOpenEx(link,
                   GDAL_OF_VECTOR, nullptr, nullptr, nullptr));

    // Display error message and exit program if dataset fails to open correctly
    if (m_dataset == nullptr)
    {
        std::cout << "Failed to open dataset." << std::endl;
        exit(1);
    }
}

void WFSData::getData(const char* name="")
{
    if (m_dataset == nullptr)
    {
        std::cout << "Dataset is not opened." << std::endl;
        return;
    }


    // Log the number of layers
        int layerCount = m_dataset->GetLayerCount();
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
  /*      // Access feature definition
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
*/
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

std::string WFSData::ExportToGeoJSON(int n, const std::string& outputFilePath)
{  
    if (m_dataset == nullptr)
    {
        std::cout << "Dataset is not opened." << std::endl;
        return "";
    }

    // Get the GeoJSON driver
    GDALDriver* geoJsonDriver = GetGDALDriverManager()->GetDriverByName("GeoJSON");
    if (geoJsonDriver == nullptr)
    {
        std::cout << "GeoJSON driver not available." << std::endl;
        return "";
    }

    // Create an temp dataset file to hold the GeoJSON output
    char** options = nullptr;
    GDALDataset* geoJsonDataset = geoJsonDriver->Create("/vsimem/temp.geojson", 0, 0, 0, GDT_Unknown, options);
    if (geoJsonDataset == nullptr)
    {
        std::cout << "Failed to create the temp GeoJSON dataset." << std::endl;
        return "";
    }

    // Get the selected layer
    OGRLayer* layer = m_dataset->GetLayer(n);
    if (layer == nullptr)
    {
        std::cout << "No layer found in the dataset." << std::endl;
        GDALClose(geoJsonDataset); // Clean up
        return "";
    }

    // Copy the layer to the GeoJSON dataset
    if (geoJsonDataset->CopyLayer(layer, layer->GetName()) == nullptr)
    {
        std::cout << "Failed to copy layer to GeoJSON dataset." << std::endl;
        GDALClose(geoJsonDataset); // Clean up
        return "";
    }

    // Close the in-memory dataset to flush data to /vsimem/
    GDALClose(geoJsonDataset);

    // Open the in-memory file
    VSILFILE* file = VSIFOpenL("/vsimem/temp.geojson", "r");
    if (file == nullptr)
    {
        std::cout << "Failed to open in-memory GeoJSON file." << std::endl;
        return "";
    }

    // Read the contents of the file
    VSIFSeekL(file, 0, SEEK_END); // Move to the end of the file to determine size
    size_t fileSize = VSIFTellL(file);
    VSIFSeekL(file, 0, SEEK_SET); // Move back to the beginning of the file

    std::string geoJsonContent(fileSize, '\0'); // Allocate a string with the required size
    VSIFReadL(&geoJsonContent[0], 1, fileSize, file);

    
    std::ofstream outputFile(outputFilePath);
        if (outputFile.is_open())
        {
            outputFile << geoJsonContent;
            outputFile.close();
            std::cout << "GeoJSON written to file: " << outputFilePath << std::endl;
        }
        else
        {
            std::cout << "Failed to write GeoJSON to file: " << outputFilePath << std::endl;
        }
    // Close and clean up
    VSIFCloseL(file);
    VSIUnlink("/vsimem/temp.geojson");

    return geoJsonContent;
}
OGRLayer* WFSData::GetLayer(const char* name)
{
    if (m_dataset == nullptr)
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