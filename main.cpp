#include "citygmlparser.h"
#include <iostream>

void printFeatureDetails(const Feature& feature) {
    std::cout << "Feature ID: " << feature.id << std::endl;

    // Display vertex count
    std::cout << "Number of vertices: " << feature.vertices.size() / 3 << std::endl;

    // Display face count
    std::cout << "Number of faces: " << feature.faces.size() / 3 << std::endl;

    // Print attributes
    std::cout << "Attributes:" << std::endl;
    for (const auto& attr : feature.attributes) {
        std::cout << "  " << attr.first << ": " << attr.second << std::endl;
    }
    std::cout << "-----------------------------" << std::endl;
}

int main() {
    CityGMLParser parser;

    // Replace this with the actual path to your CityGML file
    std::string filePath = "/home/formation/Documents/ProjetTSI/metropole-de-lyon_ima_gestion_images.imaliencitygml2018.gml";

    // Try to open the file
    if (!parser.openFile(filePath)) {
        std::cerr << "Error: Could not open CityGML file!" << std::endl;
        return -1;
    }

    // Retrieve features
    const std::vector<Feature>& features = parser.getFeatures();
    if (features.empty()) {
        std::cout << "No features found in the file!" << std::endl;
        return 0;
    }

    // Print details of each feature
    std::cout << "Parsed " << features.size() << " features:" << std::endl;
    for (const auto& feature : features) {
        printFeatureDetails(feature);
    }

    return 0;
}

