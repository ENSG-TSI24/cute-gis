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


//strucutre feature : one feature = one object
struct Feature {
    unsigned int id;
    std::string objectName;
    std::map<std::string, std::string> attributes;
    std::vector<std::vector<std::vector<glm::vec3>>> vertices;  // Scaled vertices (between -s and s)
    std::vector<std::vector<std::vector<glm::vec3>>> VerticesGeoreferenced; // Original Lambert-93 vertices (x, y, z)
    std::vector<std::vector<std::vector<glm::vec3>>> VerticesTexture;
    std::vector<std::vector<glm::vec3>> verticeNormal;

    std::tuple<double, double, double> lowerCorner; // Lower corner of bounding box (xmin, ymin)
    std::tuple<double, double, double> upperCorner; // Upper corner of bounding box (xmax, ymax)

};



class CityGMLParser {
private:
    //file dataset
    GDALDataset* dataset;
    OGRCoordinateTransformation* transform = createLambertTransformation();

    //bounding box
    float xMin, yMin, xMax, yMax, zMin, zMax;

    //object list
    std::vector<Feature> features;

public:

    //transform SRC in Lambert
    OGRCoordinateTransformation* createLambertTransformation();

    //May repair empty or null geometries
    bool executeOgr2Ogr(const std::string& inputFile, const std::string& outputFile);

    CityGMLParser();
    ~CityGMLParser();

    //returning a tuple of the vertices, the normal vertices, and the texture vertices
    std::vector<std::vector<std::vector<std::vector<glm::vec3>>>> processCoordinates(json& data);

    //opening the file
    bool openFile(const std::string& filePath);

    //filling the list of feature
    void parseFeatures();

    //export in obj
    void exportToObj(float s, const std::string& filePath);
    void exportToMtl(const std::string& filePath) const;

    //print the field of one feature
    void printFeature(const Feature& feature) const;


    //set the vertices between [-s, s]
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
