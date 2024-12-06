#include "citygmlparser.h"
#include <iostream>


int main() {
    CityGMLParser parser;

    // Replace this with the actual path to your CityGML file

    std::string inputFilePath = "/home/formation/Documents/ProjetTSI/cute-gis/SAINT_DIDIER_AU_MONT_D_OR_2015/SAINT_DIDIER_AU_MONT_D_OR_TIN_2015.gml";
    std::string outputFilePath = "/home/formation/Documents/ProjetTSI/cute-gis/cleanedtin.gml";
    if (parser.executeOgr2Ogr(inputFilePath, outputFilePath)) {
            std::cout << "ogr2ogr command executed successfully!" << std::endl;
        } else {
            std::cerr << "ogr2ogr command execution failed!" << std::endl;
        }
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
        parser.exportToObj(1.0f, "cmoimoitin.obj");
        parser.exportToMtl("cmoimoitin.mtl");

        return 0;


}
