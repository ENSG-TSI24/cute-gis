#ifndef CITY_GML_PARSER_H
#define CITY_GML_PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <gdal/ogrsf_frmts.h>
#include <glm/glm.hpp>

#include <nlohmann/json.hpp>  //sudo apt install nlohmann-json3-dev
using json = nlohmann::json;

struct Feature {
    unsigned int id;
    std::string objectName;
    std::map<std::string, std::string> attributes;
    std::vector<std::vector<std::vector<glm::vec3>>> vertices;  // Scaled vertices (between -0.85f and 0.85f)
    std::vector<std::vector<std::vector<glm::vec3>>> VerticesGeoreferenced;
    std::vector<std::vector<std::vector<glm::vec3>>> VerticesTexture;
    std::vector<std::vector<glm::vec3>> verticeNormal;// Original Lambert-93 vertices (x, y, z)
    std::vector<unsigned int> faces;
    std::tuple<double, double, double> lowerCorner; // Lower corner of bounding box (xmin, ymin)
    std::tuple<double, double, double> upperCorner; // Upper corner of bounding box (xmax, ymax)

};



class CityGMLParser {
private:
    GDALDataset* dataset;
    OGRCoordinateTransformation* transform = createLambertTransformation();


    float xMin, yMin, xMax, yMax, zMin, zMax;

    std::vector<Feature> features;

public:

        // Function declarations
    OGRCoordinateTransformation* createLambertTransformation();
    bool executeOgr2Ogr(const std::string& inputFile, const std::string& outputFile);

    CityGMLParser();
    ~CityGMLParser();
    void processCoordinates(Feature feature, json& data);
    bool openFile(const std::string& filePath);
    void parseFeatures();
    void exportToObj(float s, const std::string& filePath);
    void exportToMtl(const std::string& filePath) const;
    void printFeature(const Feature& feature) const;

    void setInScale(float s);


        // Getter methods for the envelope
        float getXMin() const;
        float getYMin() const;
        float getXMax() const;
        float getYMax() const;
        float getZMin() const;
        float getZMax() const;
        std::vector<Feature> getFeatures() const;

};

#endif // CITY_GML_PARSER_H
