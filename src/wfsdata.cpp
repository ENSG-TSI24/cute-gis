#include "wfsdata.h"
#include <iostream>
#include <ogrsf_frmts.h>

WFSData::WFSData() : m_dataset(nullptr) {}

GDALDataset* WFSData::GetDataset(){
    return this->m_dataset;
}


void WFSData::open(const char* link)
{
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

void WFSData::getData(const char* name)
{
    if (m_dataset == nullptr)
    {
        std::cout << "Dataset is not opened." << std::endl;
        return;
    }

    // Log the number of layers
    std::cout << "THE DATASET CONTAINS THIS MANY LAYERS >>>>> " << m_dataset->GetLayerCount() << std::endl;

    // Access the layer by name
    OGRLayer* layer = m_dataset->GetLayerByName(name);
    if (layer == nullptr)
    {
        std::cout << "Layer " << name <<" not found." << std::endl;
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
                std::cout << point->getX() << "," << point->getY() << std::endl;
            }
        }
        else
        {
            std::cout << "No Point Geometry" << std::endl;
        }

        // Destroy the feature
        OGRFeature::DestroyFeature(feature);
    }

    // Close the dataset
    GDALClose(m_dataset);
    m_dataset = nullptr;
}



