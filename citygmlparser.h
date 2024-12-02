#ifndef CITY_GML_PARSER_H
#define CITY_GML_PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <gdal/ogrsf_frmts.h>

struct Feature {
    unsigned int id;
    std::string objectName;
    std::map<std::string, std::string> attributes;
    std::vector<float> vertices;  // Scaled vertices (between -0.85f and 0.85f)
    std::vector<float> VerticesGeoreferenced;  // Original Lambert-93 vertices (x, y, z)
    std::vector<unsigned int> faces;
    std::pair<double, double> lowerCorner; // Lower corner of bounding box (xmin, ymin)
    std::pair<double, double> upperCorner; // Upper corner of bounding box (xmax, ymax)
};



class CityGMLParser {
private:
    GDALDataset* dataset;


    void extractGeometry(OGRGeometry* geometry, std::vector<float>& vertices,
                         std::vector<unsigned int>& faces, unsigned int& vertexOffset);

    float xMin, yMin, xMax, yMax;
    std::vector<Feature> features;

public:

    CityGMLParser();
    ~CityGMLParser();
    bool openFile(const std::string& filePath);
    void parseFeatures();
    void exportToObj(float s, const std::string& filePath);
    void exportToMtl(const std::string& filePath) const;
    void printFeature(const Feature& feature) const;
    OGRCoordinateTransformation* createLambertTransformation();
    void setInScale(float s);
    void generateEnvelope();

        // Getter methods for the envelope
        float getXMin() const;
        float getYMin() const;
        float getXMax() const;
        float getYMax() const;
        std::vector<Feature> getFeatures() const;

};

#endif // CITY_GML_PARSER_H
