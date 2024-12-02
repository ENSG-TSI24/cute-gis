#include "citygmlparser.h"

CityGMLParser::CityGMLParser() : dataset(nullptr) {
    xMin = 10000000;
    yMin = 10000000;
    xMax = 0;
    yMax = 0;
}

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


float CityGMLParser::getXMin() const {
    return xMin;
}
float CityGMLParser::getYMin() const {
    return yMin;
}
float CityGMLParser::getXMax() const {
    return xMax;
}
float CityGMLParser::getYMax() const {
    return yMax;
}
std::vector<Feature> CityGMLParser::getFeatures() const {
    return features;
}

OGRCoordinateTransformation* CityGMLParser::createLambertTransformation() {
    OGRSpatialReference* sourceSR = new OGRSpatialReference();
    sourceSR->importFromEPSG(4326); // WGS84 (Lat/Lon)

    OGRSpatialReference* targetSR = new OGRSpatialReference();
    targetSR->importFromEPSG(2154); // Lambert-93 (EPSG:2154)

    return OGRCreateCoordinateTransformation(sourceSR, targetSR);
}

void CityGMLParser::generateEnvelope() {

    // Iterate through each feature to update the envelope
    for (const Feature& feature : features) {
        // Update the minimum x and y values
        if (feature.lowerCorner.first < xMin) {
            xMin = feature.lowerCorner.first;
        }
        if (feature.lowerCorner.second < yMin) {
            yMin = feature.lowerCorner.second;
        }
        // Update the maximum x and y values
        if (feature.upperCorner.first > xMax)
        {
            xMax = feature.upperCorner.first;
        }
        if (feature.upperCorner.second > yMax) {
            yMax = feature.upperCorner.second;
        }
}

}


void CityGMLParser::extractGeometry(OGRGeometry* geometry, std::vector<float>& vertices,
                                    std::vector<unsigned int>& faces, unsigned int& vertexOffset) {
    if (!geometry) return;

    // Coordinate transformation for lat/lon to Lambert-93
    static OGRCoordinateTransformation* transform = createLambertTransformation();

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

                    // Convert lat/lon (point.getX(), point.getY()) to Lambert-93
                    double lon = point.getY();
                    double lat = point.getX();
                    double z = point.getZ(); // z (elevation) stays the same

                    // Perform the transformation
                    transform->Transform(1, &lon, &lat); // Transform lon, lat to Lambert-93 (x, y)

                    // Push transformed coordinates to vertices
                    vertices.push_back(static_cast<float>(lon));  // x (Lambert)
                    vertices.push_back(static_cast<float>(lat));  // y (Lambert)
                    vertices.push_back(static_cast<float>(z));





                    // Also store the original lat/lon values in VerticesGeoreferenced
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
    static OGRCoordinateTransformation* transform = createLambertTransformation();
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
        feature.VerticesGeoreferenced = feature.vertices;

        // Assuming lowerCorner and upperCorner are extracted from the GML bounding box
        // Example of how to set these:
        OGRGeometry* boundedBy = ogrFeature->GetGeometryRef(); // Get the bounding box geometry (Envelope)
                if (boundedBy) {
                    OGREnvelope env;
                    boundedBy->getEnvelope(&env); // Get the envelope of the bounding box

                    // Transform the lower corner
                    double lowerLon = env.MinY;
                    double lowerLat = env.MinX;
                    transform->Transform(1, &lowerLon, &lowerLat); // Transform to Lambert-93

                    // Transform the upper corner
                    double upperLon = env.MaxY;
                    double upperLat = env.MaxX;
                    transform->Transform(1, &upperLon, &upperLat); // Transform to Lambert-93

                    // Assign the transformed coordinates to feature's bounding box
                    feature.lowerCorner = std::make_pair(lowerLon, lowerLat);
                    feature.upperCorner = std::make_pair(upperLon, upperLat);

        features.push_back(feature);
        OGRFeature::DestroyFeature(ogrFeature);
    }
}

}

void CityGMLParser::setInScale(float s) {

    generateEnvelope();
    double xmin = getXMin();
    double xmax = getXMax();
    double ymin = getYMin();
    double ymax = getYMax();
    std::cout << "Generated Envelope: ("
              << xmin << ", " << ymin << ") to ("
              << xmax << ", " << ymax << ")" << std::endl;

    for (Feature& feature : features) {
        for (size_t i = 0; i < feature.vertices.size(); i += 3) {

            float originalX = feature.VerticesGeoreferenced[i];
            float originalY = feature.VerticesGeoreferenced[i + 1];
            float originalZ = feature.VerticesGeoreferenced[i + 2];

            // Scale the x and y coordinates based on the bounding box
            float scaledX = ((originalX - xmin) / (xmax - xmin)) * 2.0f * s - s;
            float scaledY = ((originalY - ymin) / (ymax - ymin)) * 2.0f * s - s;

            std::cout << "Vertice scaled: (" << scaledX << ", " << scaledY << ")" << std::endl;

            // Store the scaled values in 'vertices'
            feature.vertices[i] = scaledX;
            feature.vertices[i + 1] = scaledY;
            feature.vertices[i + 2] = originalZ; // Z remains unchanged
        }
    }
}



void CityGMLParser::exportToObj(float s, const std::string& filePath){
    setInScale(s);
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
    // Print basic feature information
    std::cout << "Feature ID: " << feature.id << std::endl;
    std::cout << "Number of vertices: " << feature.vertices.size() / 3 << std::endl;
    std::cout << "Number of faces: " << feature.faces.size() / 3 << std::endl;

    // Print the first two and last two vertices
    std::cout << "First 2 vertices: ";
    for (int i = 0; i < 2 && i * 3 + 2 < feature.vertices.size(); ++i) {
        std::cout << "(" << feature.vertices[i * 3] << ", "
                  << feature.vertices[i * 3 + 1] << ", "
                  << feature.vertices[i * 3 + 2] << ") ";
    }

    std::cout << "\nLast 2 vertices: ";
    for (int i = feature.vertices.size() / 3 - 2; i < feature.vertices.size() / 3 && i * 3 + 2 < feature.vertices.size(); ++i) {
        std::cout << "(" << feature.vertices[i * 3] << ", "
                  << feature.vertices[i * 3 + 1] << ", "
                  << feature.vertices[i * 3 + 2] << ") ";
    }
    std::cout << std::endl;

    // Print the first two and last two Georeferenced Vertices
    std::cout << "First 2 Georeferenced Vertices: ";
    for (int i = 0; i < 2 && i * 3 + 2 < feature.VerticesGeoreferenced.size(); ++i) {
        std::cout << "(" << feature.VerticesGeoreferenced[i * 3] << ", "
                  << feature.VerticesGeoreferenced[i * 3 + 1] << ", "
                  << feature.VerticesGeoreferenced[i * 3 + 2] << ") ";
    }

    std::cout << "\nLast 2 Georeferenced Vertices : ";
    for (int i = feature.VerticesGeoreferenced.size() / 3 - 2; i < feature.VerticesGeoreferenced.size() / 3 && i * 3 + 2 < feature.vertices.size(); ++i) {
        std::cout << "(" << feature.VerticesGeoreferenced[i * 3] << ", "
                  << feature.VerticesGeoreferenced[i * 3 + 1] << ", "
                  << feature.VerticesGeoreferenced[i * 3 + 2] << ") ";
    }
    std::cout << std::endl;

    // Print attributes
    std::cout << "Attributes: " << std::endl;
    for (const auto& [key, value] : feature.attributes) {
        std::cout << "  " << key << ": " << value << std::endl;
    }

    // Print the bounding box (lower and upper corners)
    std::cout << "Lower Corner: ("
              << feature.lowerCorner.first << ", " // Accessing first value of the pair (x)
              << feature.lowerCorner.second << ")" << std::endl; // Accessing second value of the pair (y)

    std::cout << "Upper Corner: ("
              << feature.upperCorner.first << ", "  // Accessing first value of the pair (x)
              << feature.upperCorner.second << ")" << std::endl; // Accessing second value of the pair (y)

    std::cout << "-----------------------------" << std::endl;
}


