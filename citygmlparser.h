#ifndef CITY_GML_PARSER_H
#define CITY_GML_PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <gdal/ogrsf_frmts.h>

// Feature struct to hold geometry and attributes
struct Feature {
    std::string id;
    std::string objectName;
    std::vector<float> vertices; // x, y, z triplets
    std::vector<unsigned int> faces; // indices for faces
    std::map<std::string, std::string> attributes; // Key-value pairs for attributes
};

class CityGMLParser {
private:
    GDALDataset* dataset;


    void extractGeometry(OGRGeometry* geometry, std::vector<float>& vertices,
                         std::vector<unsigned int>& faces, unsigned int& vertexOffset);

public:
    std::vector<Feature> features;
    CityGMLParser();
    ~CityGMLParser();
    bool openFile(const std::string& filePath);
    void parseFeatures();
    void exportToObj(const std::string& filePath) const;
    void exportToMtl(const std::string& filePath) const;
    void printFeature(const Feature& feature) const;
};

#endif // CITY_GML_PARSER_H
