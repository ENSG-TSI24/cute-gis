#include "citygmlparser.h"
#include <iostream>


int main() {
    CityGMLParser parser;

    // Replace this with the actual path to your CityGML file
    std::string inputFilePath = "/home/formation/Documents/ProjetTSI/SAINT_DIDIER_AU_MONT_D_OR_2015/SAINT_DIDIER_AU_MONT_D_OR_EAU_2015.gml";
        if (!parser.openFile(inputFilePath)) {
            return 1;
        }

        parser.parseFeatures();

        for (const auto& feature : parser.getFeatures()) {
            parser.printFeature(feature);
        }


        parser.setInScale(0.85f);

        for (const auto& feature : parser.getFeatures()) {
            parser.printFeature(feature);
        }
        parser.exportToObj(1.0f, "cmoimoi.obj");
        parser.exportToMtl("cmoimoi.mtl");

        return 0;


}
