#include "citygmlparser.h"
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <nlohmann/json.hpp>  //sudo apt install nlohmann-json3-dev
using json = nlohmann::json;

CityGMLParser::CityGMLParser() : dataset(nullptr) {
    xMin = 10000000;
    yMin = 10000000;
    zMin = 10000000;
    xMax = 0;
    yMax = 0;
    zMax = 0;
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
float CityGMLParser::getZMax() const {
    return zMax;
}
float CityGMLParser::getZMin() const {
    return zMin;
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
    //Nombre de feature dans le CityGML
    int nbfeatures = layer -> GetFeatureCount();


    //Boucle sur les features
    for (int i=0; i<nbfeatures; i++){
        Feature feature;
        OGRFeature* ogrfeature = layer->GetNextFeature();


        OGRGeometry* geometry = ogrfeature->GetGeometryRef();
        if (geometry-> IsEmpty()){
            std::cout << "geometry is null" << std::endl;

        }
        else {


       feature.id = ogrfeature->GetFID();
       feature.objectName = "Feature_" + std::to_string(ogrfeature->GetFID());

    // Parse attributes
        for (int i = 0; i < ogrfeature->GetFieldCount(); i++) {
            const char* fieldName = ogrfeature->GetFieldDefnRef(i)->GetNameRef();
            const char* fieldValue = ogrfeature->GetFieldAsString(i);
            feature.attributes[fieldName] = fieldValue;
        }

        // Récupération de la géométrie
/*
        char* wkt = nullptr;
        geometry->exportToWkt(&wkt);

        std::cout << wkt << std::endl;
        CPLFree(wkt);
*/
        std::string tab;

        if (geometry && std::string(geometry->getGeometryName()) != "MULTIPOLYGON") {
            OGRGeometry* simplifiedGeometry = geometry->clone(); // Clone the geometry
            OGRGeometry* converted = OGRGeometryFactory::forceToMultiPolygon(simplifiedGeometry);
            tab = converted -> exportToJson();

        }
        else {
        tab = geometry -> exportToJson();
        }






        // Réparation de la géométrie si elle est cassée
        json jsondata = json::parse(tab);




        // Passage des vertices à la feature

        std::vector<std::vector<std::vector<std::vector<glm::vec3>>>> data = processCoordinates(jsondata);
        feature.vertices = data.at(0);
        feature.VerticesGeoreferenced = data.at(0);
        feature.verticeNormal = data.at(1).at(0);
        feature.VerticesTexture = data.at(2);


        // Passage des vertices à la feature
        std::cout<< "feature" <<to_string(feature.vertices.at(0).at(0).at(0)) << std::endl;





        //Passage de l'enveloppe à la feature
        feature.lowerCorner = std::make_tuple(xMin,yMin,zMin);
        feature.upperCorner = std::make_tuple(xMax,yMax,zMax);

        //Ajout de la feature à la liste des features
        features.push_back(feature);
        OGRFeature::DestroyFeature(ogrfeature);


    }

}
}


std::vector<std::vector<std::vector<std::vector<glm::vec3>>>> CityGMLParser::processCoordinates(json& data) {
    std::vector<std::vector<std::vector<glm::vec3>>> multipolygonList;
    std::vector<std::vector<std::vector<glm::vec3>>> normalsListUniform;
    std::vector<std::vector<glm::vec3>> normalsList;
    std::vector<std::vector<std::vector<glm::vec3>>> multipolygontexture;
    if (data.contains("coordinates")) {

        for (auto& multiPolygon : data["coordinates"]) {
            std::vector<std::vector<glm::vec3>> polygonList;

            for (auto& polygon : multiPolygon) {
                // Flatten the list of 2D points into a single list of floats
                std::vector<float> flatList;
                for (auto& coordinate : polygon) {
                    for (auto& value : coordinate) {
                        flatList.push_back(value);
                    }
                }

                // Parse the flat list into 3D points
                std::vector<glm::vec3> points3D;
                for (size_t i = 0; i < flatList.size(); i += 3) {
                    float x = flatList[i];
                    float y = flatList[i + 1];
                    float z = flatList[i + 2];
                    points3D.push_back({x, y, z});
                }






                // Replace the original polygon with the new list of 3D points
                polygonList.push_back(points3D);

            }
            std::vector<glm::vec3> normals3D;
            for (const auto& polygon : polygonList) {
                for (size_t i = 0; i < polygon.size(); ++i) {
                    glm::vec3 v1 = polygon[i];
                    glm::vec3 v2 = polygon[(i + 1) % polygon.size()];
                    glm::vec3 v3 = polygon[(i + 2) % polygon.size()];

                    glm::vec3 u = v2 - v1;
                    glm::vec3 v = v3 - v1;

                    if (u!= glm::vec3(0,0,0) && v!= glm::vec3(0,0,0) && u!=v){
                    glm::vec3 normal = glm::normalize(glm::cross(u, v));

                    normals3D.push_back(normal); // Accumuler les normales

                    }
                }
            }


            multipolygonList.push_back(polygonList);
            normalsList.push_back(normals3D);
        }

        for (int j = 0; j< multipolygonList.size();j++){
            for (int k =0; k< multipolygonList.at(j).size(); k++){
                for (int l =0; l< multipolygonList.at(j).at(k).size(); l++){
                    if (multipolygonList.at(j).at(k).at(l).x < xMin) {  // Get X from lowerCorner
                        xMin = multipolygonList.at(j).at(k).at(l).x;  // Set X as minimum
                    }
                    if (multipolygonList.at(j).at(k).at(l).y < yMin) {  // Get Y from lowerCorner
                        yMin = multipolygonList.at(j).at(k).at(l).y;  // Set Y as minimum
                    }
                    if (multipolygonList.at(j).at(k).at(l).z < zMin) {  // Get Z from upperCorner
                        zMin = multipolygonList.at(j).at(k).at(l).z;  // Set Z as minimum
                    }

                    // Update the maximum x and y and z values
                    if (multipolygonList.at(j).at(k).at(l).x > xMax) {  // Get X from upperCorner
                        xMax = multipolygonList.at(j).at(k).at(l).x;  // Set X as maximum
                    }
                    if (multipolygonList.at(j).at(k).at(l).y > yMax) {  // Get Y from upperCorner
                        yMax = multipolygonList.at(j).at(k).at(l).y;  // Set Y as maximum
                    }
                    if (multipolygonList.at(j).at(k).at(l).z > zMax) {  // Get Z from upperCorner
                        zMax = multipolygonList.at(j).at(k).at(l).z;  // Set Z as maximum
                    }

                }
            }
        }

        for (int j = 0; j< multipolygonList.size();j++){
            std::vector<std::vector<glm::vec3>> polygontexture;
            for (int k =0; k< multipolygonList.at(j).size(); k++){
                std::vector<glm::vec3> textureCoords;
                for (int l =0; l< multipolygonList.at(j).at(k).size(); l++){
                    float u = (multipolygonList.at(j).at(k).at(l).x - xMin) / (xMax - xMin);
                    float v = (multipolygonList.at(j).at(k).at(l).y - yMin) / (yMax - yMin);
                    float w = (multipolygonList.at(j).at(k).at(l).z - zMin) / (zMax - zMin);
                    textureCoords.emplace_back(u, v, w);
                }
                polygontexture.push_back(textureCoords);
             }
            multipolygontexture.push_back(polygontexture);
        }

        std::vector<std::vector<std::vector<std::vector<glm::vec3>>>> returnTuple;
        returnTuple.push_back(multipolygonList);

        normalsListUniform.push_back(normalsList);
        returnTuple.push_back(normalsListUniform);

        returnTuple.push_back(multipolygontexture);

        return returnTuple;
    }
}

bool CityGMLParser::executeOgr2Ogr(const std::string& inputFile, const std::string& outputFile) {
    // Construct the ogr2ogr command
    std::string command = "ogr2ogr -f GML " + outputFile + " " + inputFile;

    // Print the command for debugging
    std::cout << "Executing command: " << command << std::endl;

    // Execute the command
    int returnCode = std::system(command.c_str());

    // Check the result
    if (returnCode != 0 || returnCode != 35584) {
        std::cerr << "Error: ogr2ogr command failed with return code " << returnCode << std::endl;
        return false;
    }

    return true;
}

void CityGMLParser::setInScale(float s) {


    std::cout << "Generated Envelope: ("
              << xMin << ", " << yMin << ", " << zMin << ")  ("
              << xMax << ", " << yMax << ", " << zMax << ")" << std::endl;

    for (Feature& feature : features) {
        for (int i = 0; i < feature.vertices.size(); i += 3) {
            for (int k =0; k< feature.vertices.at(i).size(); k++){
                for (int l =0; l<  feature.vertices.at(i).at(k).size(); l++){


            //Original value of x,y,z
            float originalX = feature.VerticesGeoreferenced.at(i).at(k).at(l).x;
            float originalY = feature.VerticesGeoreferenced.at(i).at(k).at(l).y;
            float originalZ = feature.VerticesGeoreferenced.at(i).at(k).at(l).z;

            // Scale the x, y, z  coordinates based on the bounding box
            float scaledX = ((originalX - xMin) / (xMax - xMin)) * 2.0f * s - s;
            float scaledY = ((originalY - yMin) / (yMax - yMin)) * 2.0f * s - s;
            float scaledZ = ((originalZ - zMin) / (zMax - zMin)) * 2.0f * s - s;

         /*   std::cout << "Vertice scaled: (" << scaledX << ", " << scaledY << ", " << scaledZ << ")" << std::endl;
            std::cout << "Original Vertice : (" << originalX << ", " << originalY << ", " << originalZ << ")" << std::endl;
*/

            // Store the scaled values in 'vertices'
            feature.vertices.at(i).at(k).at(l).x = scaledX;
            feature.vertices.at(i).at(k).at(l).y = scaledY;
            feature.vertices.at(i).at(k).at(l).z = scaledZ;
                }
            }
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
        if (feature.vertices.empty()) {
            std::cerr << "Warning: Feature " << feature.id << " has no geometry and will be skipped." << std::endl;
            continue;
        }
        std::cout<< "je fais un objet" << std::endl;
        objFile << "o " << feature.objectName << "\n";
        objFile << "usemtl Material_" << feature.id << "\n"; // Assign material

        for (int i = 0; i < feature.vertices.size(); i += 3) {
            for (int k =0; k< feature.vertices.at(i).size(); k++){
                int n = 1;
                for (int l =0; l<  feature.vertices.at(i).at(k).size(); l++){

                    std::cout<<"v :" <<to_string(feature.vertices.at(i).at(k).at(l))<<std::endl;
                    objFile << "v " << feature.vertices.at(i).at(k).at(l).x << " "
                            << feature.vertices.at(i).at(k).at(l).y << " "
                            << feature.vertices.at(i).at(k).at(l).z << "\n";
                    std::cout<<"vt : "<<to_string(feature.VerticesTexture.at(i).at(k).at(l))<<std::endl;
                    objFile << "vt " << feature.VerticesTexture.at(i).at(k).at(l).x << " "
                            << feature.VerticesTexture.at(i).at(k).at(l).y << " "
                            << feature.VerticesTexture.at(i).at(k).at(l).z << "\n";


                }
                if (feature.verticeNormal.at(i).size() >= k-1){
                std::cout<< "vn :" <<to_string(feature.verticeNormal.at(i).at(k)) << std::endl;
                objFile << "vn " << feature.verticeNormal.at(i).at(k).x << " "
                        << feature.verticeNormal.at(i).at(k).y << " "
                        << feature.verticeNormal.at(i).at(k).z << "\n";
                }
                objFile << "f ";


                while (n < feature.vertices.at(i).size() ){
                    objFile << -n << "/" << -n << "/" << -1 << " ";
                    n+=1;
                }
                objFile << "\n";
                }


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
    std::cout << "Number of vertices: " << feature.vertices.size()  << std::endl;


    // Print the first two and last two vertices
    std::cout << "First 3 vertices: ";

    std::cout << "(" << glm::to_string(feature.vertices.at(0).at(0).at(0)) << ", "
              << glm::to_string(feature.vertices.at(0).at(0).at(1)) << ", "
              << glm::to_string(feature.vertices.at(0).at(0).at(2)) << ") ";


    std::cout << "\nLast 2 vertices: ";

    int i = feature.vertices.size();
    int j = feature.vertices.at(0).size();
    int k = feature.vertices.at(0).at(0).size();

    std::cout << "(" << glm::to_string(feature.vertices.at(i-1).at(j-1).at(k-3)) << ", "
              << glm::to_string(feature.vertices.at(i-1).at(j-1).at(k-2)) << ", "
              << glm::to_string(feature.vertices.at(i-1).at(j-1).at(k-1)) << ") ";

    std::cout << std::endl;

    // Print the first two and last two georeferenced vertices
    std::cout << "First 3 vertices: ";

    std::cout << "(" << glm::to_string(feature.VerticesGeoreferenced.at(0).at(0).at(0)) << ", "
              << glm::to_string(feature.VerticesGeoreferenced.at(0).at(0).at(1)) << ", "
              << glm::to_string(feature.VerticesGeoreferenced.at(0).at(0).at(2)) << ") ";


    std::cout << "\nLast 3 vertices: ";

    std::cout << "(" << glm::to_string(feature.VerticesGeoreferenced.at(i-1).at(j-1).at(k-3)) << ", "
              << glm::to_string(feature.VerticesGeoreferenced.at(i-1).at(j-1).at(k-2)) << ", "
              << glm::to_string(feature.VerticesGeoreferenced.at(i-1).at(j-1).at(k-1)) << ") ";

    std::cout << std::endl;
/*
   // Print attributes
    std::cout << "Attributes: " << std::endl;
    for (const auto& [key, value] : feature.attributes) {
        std::cout << "  " << key << ": " << value << std::endl;
    }
*/
    // Print the bounding box (lower and upper corners)
    std::cout << "Lower Corner: ("
              << std::get<0>(feature.lowerCorner) << ", " // Accessing first value of the pair (x)
              << std::get<1>(feature.lowerCorner) << ", "
              << std::get<2>(feature.lowerCorner) << ")" << std::endl; // Accessing second value of the pair (y)

    std::cout << "Upper Corner: ("
              << std::get<0>(feature.upperCorner) << ", " // Accessing first value of the pair (x)
              << std::get<1>(feature.upperCorner) << ", "
              << std::get<2>(feature.upperCorner) << ")" << std::endl; // Accessing second value of the pair (y)


    std::cout << "-----------------------------" << std::endl;
}

