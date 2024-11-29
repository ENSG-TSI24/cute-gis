#include "citygmlarser.h"

CityGMLParser::CityGMLParser() : dataset(nullptr) {}

CityGMLParser::~CityGMLParser() {
    if (dataset) {
        GDALClose(dataset);
    }
}

bool CityGMLParser::openFile(const std::string& filePath) {
    GDALAllRegister();
    dataset = static_cast<GDALDataset*>(GDALOpenEx(filePath.c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr));
    if (!dataset) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return false;
    }
    return true;
}

void CityGMLParser::extractGeometry(OGRGeometry* geometry, std::vector<float>& vertices,
                                    std::vector<unsigned int>& faces, unsigned int& vertexOffset) {
    if (!geometry) return;

    OGRwkbGeometryType geomType = geometry->getGeometryType();

    if (geomType == wkbMultiSurface || geomType == wkbMultiPolygon) {
        OGRMultiSurface* multiSurface = geometry->toMultiSurface();
        if (multiSurface) {
            for (auto i = 0; i < multiSurface->getNumGeometries(); i++) {
                OGRGeometry* subGeometry = multiSurface->getGeometryRef(i);
                extractGeometry(subGeometry, vertices, faces, vertexOffset);
            }
        }
    } else if (geomType == wkbPolygon) {
        OGRPolygon* polygon = geometry->toPolygon();
        if (polygon) {
            OGRLinearRing* ring = polygon->getExteriorRing();
            if (ring) {
                unsigned int startVertex = vertices.size() / 3;
                for (int i = 0; i < ring->getNumPoints(); i++) {
                    OGRPoint point;
                    ring->getPoint(i, &point);
                    vertices.push_back(static_cast<float>(point.getX()));
                    vertices.push_back(static_cast<float>(point.getY()));
                    vertices.push_back(static_cast<float>(point.getZ()));
                }
                for (int i = 1; i < ring->getNumPoints() - 1; i++) {
                    faces.push_back(startVertex);
                    faces.push_back(startVertex + i);
                    faces.push_back(startVertex + i + 1);
                }
            }
        }
    } else {
        std::cerr << "Unsupported geometry type: " << geometry->getGeometryName() << std::endl;
    }
}

void CityGMLParser::parseFeatures() {
    features.clear();

    if (!dataset) {
        std::cerr << "No dataset loaded for parsing!" << std::endl;
        return;
    }

    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        std::cerr << "No layers found in the CityGML file!" << std::endl;
        return;
    }

    OGRFeature* ogrFeature;
    unsigned int vertexOffset = 0;
    while ((ogrFeature = layer->GetNextFeature()) != nullptr) {
        Feature feature;
        feature.id = ogrFeature->GetFID();
        feature.objectName = "Feature_" + std::to_string(ogrFeature->GetFID());

        // Parse attributes
        for (int i = 0; i < ogrFeature->GetFieldCount(); i++) {
            const char* fieldName = ogrFeature->GetFieldDefnRef(i)->GetNameRef();
            const char* fieldValue = ogrFeature->GetFieldAsString(i);
            feature.attributes[fieldName] = fieldValue;
        }

        // Parse geometry
        OGRGeometry* geometry = ogrFeature->GetGeometryRef();
        extractGeometry(geometry, feature.vertices, feature.faces, vertexOffset);
        vertexOffset += feature.vertices.size() / 3;

        features.push_back(feature);
        OGRFeature::DestroyFeature(ogrFeature);
    }
}

void CityGMLParser::exportToObj(const std::string& filePath) const {
    std::ofstream objFile(filePath);
    if (!objFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filePath << std::endl;
        return;
    }

    unsigned int vertexOffset = 1;
    for (const auto& feature : features) {
        if (feature.vertices.empty() || feature.faces.empty()) {
            std::cerr << "Warning: Feature " << feature.id << " has no geometry and will be skipped." << std::endl;
            continue;
        }

        objFile << "o " << feature.objectName << "\n";

        for (size_t i = 0; i < feature.vertices.size(); i += 3) {
            objFile << "v " << feature.vertices[i] << " "
                    << feature.vertices[i + 1] << " "
                    << feature.vertices[i + 2] << "\n";
        }

        for (size_t i = 0; i < feature.faces.size(); i += 3) {
            objFile << "f " << (feature.faces[i] + vertexOffset) << " "
                    << (feature.faces[i + 1] + vertexOffset) << " "
                    << (feature.faces[i + 2] + vertexOffset) << "\n";
        }

        vertexOffset += feature.vertices.size() / 3;
        objFile << "\n";
    }

    objFile.close();
    std::cout << "Exported features to OBJ file: " << filePath << std::endl;
}

void CityGMLParser::printFeature(const Feature& feature) const {
    std::cout << "Feature ID: " << feature.id << std::endl;
    std::cout << "Number of vertices: " << feature.vertices.size() / 3 << std::endl;
    std::cout << "Number of faces: " << feature.faces.size() / 3 << std::endl;
    std::cout << "Attributes: " << std::endl;
    for (const auto& [key, value] : feature.attributes) {
        std::cout << "  " << key << ": " << value << std::endl;
    }
    std::cout << "-----------------------------" << std::endl;
}
