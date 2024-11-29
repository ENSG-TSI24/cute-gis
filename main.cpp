#include "citygmlparser.h"
#include <iostream>

int main() {
    CityGMLParser parser;

    // Replace this with the actual path to your CityGML file
    std::string inputFilePath = "/home/formation/Documents/ProjetTSI/metropole-de-lyon_ima_gestion_images.imaliencitygml2018.gml";

        if (!parser.openFile(inputFilePath)) {
            return 1;
        }

        parser.parseFeatures();

        for (const auto& feature : parser.features) {
            parser.printFeature(feature);
        }

        parser.exportToObj("output.obj");
        parser.exportToMtl("output.mtl");

        return 0;

    return 0;
}



