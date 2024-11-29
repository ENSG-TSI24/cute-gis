#include "citygmlparser.h"

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

    std::string mtlFileName = filePath.substr(0, filePath.find_last_of('.')) + ".mtl";
    objFile << "mtllib " << mtlFileName << "\n"; // Reference the .mtl file

    unsigned int vertexOffset = 1;
    for (const auto& feature : features) {
        if (feature.vertices.empty() || feature.faces.empty()) {
            std::cerr << "Warning: Feature " << feature.id << " has no geometry and will be skipped." << std::endl;
            continue;
        }

        objFile << "o " << feature.objectName << "\n";
        objFile << "usemtl Material_" << feature.id << "\n"; // Assign material

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


void CityGMLParser::exportToMtl(const std::string& filePath) const {
    std::ofstream mtlFile(filePath);
    if (!mtlFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filePath << std::endl;
        return;
    }

    for (const auto& feature : features) {
        std::string materialName = "Material_" + feature.id;

        // Default material properties
        float ambient[3] = {1.0f, 1.0f, 1.0f}; // Default white
        float diffuse[3] = {0.8f, 0.8f, 0.8f}; // Default light gray
        float specular[3] = {0.5f, 0.5f, 0.5f}; // Default gray
        float opacity = 1.0f;                  // Default opaque
        float shininess = 10.0f;               // Default shininess

        // Check and assign values based on feature attributes
        if (feature.attributes.count("libofficiel")) {
            // Generate unique color from 'libofficiel'
            auto colorHash = [](const std::string& str) -> unsigned int {
                unsigned int hash = 0;
                for (char c : str) hash = (hash * 31) + c;
                return hash;
            };
            unsigned int hash = colorHash(feature.attributes.at("libofficiel"));
            ambient[0] = ((hash & 0xFF0000) >> 16) / 255.0f;
            ambient[1] = ((hash & 0x00FF00) >> 8) / 255.0f;
            ambient[2] = (hash & 0x0000FF) / 255.0f;
            diffuse[0] = ambient[0] * 0.8f;
            diffuse[1] = ambient[1] * 0.8f;
            diffuse[2] = ambient[2] * 0.8f;
        } else {
            std::cerr << "Warning: 'libofficiel' missing for feature " << feature.id
                      << ", using default Ka and Kd.\n";
        }

        if (feature.attributes.count("gid")) {
            // Customize shininess based on 'gid'
            shininess = (std::stoi(feature.attributes.at("gid")) % 50) + 10.0f;
        } else {
            std::cerr << "Warning: 'gid' missing for feature " << feature.id
                      << ", using default Ns.\n";
        }

        if (feature.attributes.count("uid")) {
            // Customize opacity based on 'uid'
            opacity = (std::stoi(feature.attributes.at("uid")) % 100) / 100.0f + 0.5f;
            if (opacity > 1.0f) opacity = 1.0f;
        } else {
            std::cerr << "Warning: 'uid' missing for feature " << feature.id
                      << ", using default opacity.\n";
        }

        // Write material properties to the MTL file
        mtlFile << "newmtl " << materialName << "\n";
        mtlFile << "Ka " << ambient[0] << " " << ambient[1] << " " << ambient[2] << "\n";
        mtlFile << "Kd " << diffuse[0] << " " << diffuse[1] << " " << diffuse[2] << "\n";
        mtlFile << "Ks " << specular[0] << " " << specular[1] << " " << specular[2] << "\n";
        mtlFile << "d " << opacity << "\n";
        mtlFile << "Ns " << shininess << "\n";
        mtlFile << "\n";
    }

    mtlFile.close();
    std::cout << "Exported materials to MTL file: " << filePath << std::endl;
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
