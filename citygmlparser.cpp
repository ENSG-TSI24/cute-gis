#include "citygmlparser.h"
#include <iostream>

// Constructor
CityGMLParser::CityGMLParser() : dataset(nullptr) {
    // Register GDAL drivers
    GDALAllRegister();
}

// Destructor
CityGMLParser::~CityGMLParser() {
    // Close the dataset if open
    if (dataset) {
        GDALClose(dataset);
        dataset = nullptr;
    }
}

// Open and load a CityGML file
bool CityGMLParser::openFile(const std::string& filePath) {
    // Close any previously open dataset
    if (dataset) {
        GDALClose(dataset);
        dataset = nullptr;
    }

    // Open the CityGML file
    dataset = static_cast<GDALDataset*>(GDALOpenEx(filePath.c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr));
    if (!dataset) {
        std::cerr << "Failed to open CityGML file: " << filePath << std::endl;
        return false;
    }

    // Parse the features
    parseFeatures();
    return true;
}

// Retrieve all parsed features
const std::vector<Feature>& CityGMLParser::getFeatures() const {
    return features;
}

// Parse features from the dataset
void CityGMLParser::parseFeatures() {
    features.clear();

    if (!dataset) {
        std::cerr << "No dataset loaded for parsing!" << std::endl;
        return;
    }

    // Get the first layer
    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        std::cerr << "No layers found in the CityGML file!" << std::endl;
        return;
    }

    // Iterate through features
    OGRFeature* ogrFeature;
    unsigned int vertexOffset = 0; // For global vertex indexing
    while ((ogrFeature = layer->GetNextFeature()) != nullptr) {
        Feature feature;

        // Assign a unique ID
        feature.id = "Feature_" + std::to_string(ogrFeature->GetFID());

        // Extract geometry
        extractGeometry(ogrFeature->GetGeometryRef(), feature.vertices, feature.faces, vertexOffset);

        // Extract metadata
        for (int i = 0; i < ogrFeature->GetFieldCount(); ++i) {
            const char* fieldName = ogrFeature->GetFieldDefnRef(i)->GetNameRef();
            const char* fieldValue = ogrFeature->GetFieldAsString(i);
            feature.attributes.emplace_back(fieldName, fieldValue);
        }

        features.push_back(feature);

        // Destroy the feature to free memory
        OGRFeature::DestroyFeature(ogrFeature);
    }
}

// Extract geometry into vertices and faces
void CityGMLParser::extractGeometry(OGRGeometry* geometry, std::vector<float>& vertices,
                                    std::vector<unsigned int>& faces, unsigned int& vertexOffset) {
    if (!geometry)
        return;

    if (geometry->getGeometryType() == wkbPolygon || geometry->getGeometryType() == wkbMultiPolygon) {
        OGRPolygon* polygon = dynamic_cast<OGRPolygon*>(geometry);
        if (!polygon)
            return;

        OGRLinearRing* ring = polygon->getExteriorRing();
        std::vector<unsigned int> faceIndices;
        for (int i = 0; i < ring->getNumPoints(); ++i) {
            double x = ring->getX(i);
            double y = ring->getY(i);
            double z = ring->getZ(i);
            vertices.push_back(static_cast<float>(x));
            vertices.push_back(static_cast<float>(y));
            vertices.push_back(static_cast<float>(z));
            faceIndices.push_back(vertexOffset++);
        }
        if (faceIndices.size() >= 3) {
            for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
                faces.push_back(faceIndices[0]);
                faces.push_back(faceIndices[i]);
                faces.push_back(faceIndices[i + 1]);
            }
        }
    }
}
