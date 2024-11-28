#include "citygmlparser.h"
#include <iostream>

int main() {
    CityGMLParser parser;

    // Replace this with the actual path to your CityGML file
    std::string inputFilePath = "/home/formation/Documents/ProjetTSI/metropole-de-lyon_ima_gestion_images.imaliencitygml2018.gml";

    std::string outputFilePath = "testParsing.obj";

    // Try to open the file
    if (!parser.openFile(inputFilePath)) {
        std::cerr << "Error: Could not open CityGML file!" << std::endl;
        return -1;
    }

    // Print features
    const auto& features = parser.getFeatures();
    if (features.empty()) {
        std::cout << "No features found in the file!" << std::endl;
        return 0;
    }

    for (const auto& feature : features) {
        parser.printFeature(feature);
    }

    // Export to OBJ
    parser.exportToObj(outputFilePath);

    return 0;
}



